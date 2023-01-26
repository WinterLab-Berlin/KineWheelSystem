# Repository Contents

```
|   kine-wheel-arena.yaml
├───Arduino
|   ├───cam_and_light_sync
│   └───KWA-Controller
├───Camera
|       acA720-520uc-inference.pfs
├───DLC-Skeleton
│   ├───dlc-models
│   ├───evaluation-results
│   ├───labeled-data
│   ├───training-datasets
│   └───videos
├───Docs
└───Inference
        inference.ipynb
        inference.py
```

# Installation

Steps described in this section only need to be done once.
This guide assumes that [Anaconda/Miniconda](Requirements.md#anacondaminiconda) is used to manage Python and its packages.

First, clone the [KineWheelSystem repository](https://github.com/WinterLab-Berlin/KineWheelSystem) from GitHub.
Alternatively, GitHub offers a [link](https://github.com/WinterLab-Berlin/KineWheelSystem/archive/refs/heads/main.zip) to download the repository as a ZIP file.

In the example below, the repository is cloned to `C:/Data/KineWheelSystem`.
You can choose any valid path; however, the destination folder (here: `KineWheelSystem`) must not exist or be empty.

```console
git clone https://github.com/WinterLab-Berlin/KineWheelSystem.git C:\Data\KineWheelSystem
```

[Open a Anaconda prompt](https://docs.conda.io/projects/conda/en/latest/user-guide/getting-started.html#starting-conda) (type *anaconda* into the Windows search box) and navigate to the repository root folder `KineWheelSystem`.

```console
cd C:\Data\KineWheelSystem
```

Create a conda (virtual Python) environment with all the required packages by running the following command:

```console
conda env create -f kine-wheel-arena.yaml
```

Whenever opening a new Anaconda prompt, usage of the new environment requires activation, running the command below:

```console
conda activate kwa
```

The environment can be deactivated by running:

```console
conda deactivate
```
