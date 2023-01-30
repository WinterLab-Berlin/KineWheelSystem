# Example

The following example shows how to use the pre-trained neural network on an unlabeled sample video.
To follow along, ensure that you have read the [Installation](Installation.md#installation) section and set up the required environment.

First, download the sample video `2022-08-19T14-07-11--Basler acA720-520uc-x264.mp4` (68 MiB) from [![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.7586688.svg)](https://doi.org/10.5281/zenodo.7586688).

> **Note:** The video is about 44 seconds long and shows a mouse running on a wheel.
> The playback speed of the video has been slowed down by a factor of ~30 for illustrative purposes.

Next, open an Anaconda terminal and activate the *KineWheelSystem* Anaconda environment:

```console
conda activate kwa
```

In the Anaconda terminal, navigate to the `Inference` folder in the repository root folder.

```console
cd C:\repository\root\KineWheelSystem\Inference
```

> **Note:** Adapt the command lines below to reflect the proper locations of the downloaded sample video and the DLC config file on your system.

Run `inference.py` to predict paw locations in the sample video using the pre-trained neural network.

```console
python inference.py predict -v "c:\path\to\sample\2022-08-19t14-07-11--basler aca720-520uc-x264.mp4" -c "c:\repository\root\dlc-skeleton\config.yaml"
```

Finally, create the labeled video based on the predictions made above.

```console
python inference.py label -v "c:\path\to\sample\2022-08-19t14-07-11--basler aca720-520uc-x264.mp4" -c "c:\repository\root\dlc-skeleton\config.yaml"
```

The labeled video (as well as the raw predictions in CSV and HDF5 format) can be found in the folder of the sample video.

For reference purposes, the labeled video and raw predictions are also provided on Zenodo (see link on top of page).
