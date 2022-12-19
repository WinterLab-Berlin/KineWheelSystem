"""
This command-line program can perform two operations:
    1) predict mouse paw locations in a video using a deep neural network and
    2) label a video for visualization purposes, based on its prediction results.

Prediction results are stored as HDF5 and CSV file in the same folder where
the corresponding video resides. 

Below are 3 examples on how to call the program. The general format is:

```console
python inference.py <operation> <flags>
```

Operations and flags are described in the program's help message.

Example 1: Print Help

Prints the program's help text to the console.

```console
python inference.py -h
```

Example 2: Prediction

Predict the paw locations for each video in `video_folder`, using the deep
neural network specified in `config.yaml`.

```console
python inference.py predict -c /path/to/dlc/config.yaml -v /path/to/video_folder
```

Example 3: Video Labelling

Label the paw locations in `video_file.mp4` as predicted by the neural network.
This call assumes that the prediction results for `video_file.mp4` are stored in
the same folder where the video resides. The labeled video will be saved to this
folder as well.

```console
python inference.py label -c /path/to/dlc/config.yaml -v /path/to/video_file.mp4
```


Date modified: Dec 12, 2022
"""

import argparse
from argparse import ArgumentParser, Namespace
import logging  # Debugging
import os
from pathlib import Path
from typing import Any, Dict, List, Sequence, Tuple
import yaml

# GUIs don't work on the cloud, so label your data locally on your computer!
# This will suppress the GUI support
os.environ["DLClight"]="True"


logging.basicConfig(format='%(levelname)s:%(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)


def predict(cfg_path: str, video_paths: Tuple[str, List[str]]):
    """
    Predict paw locations frame by frame in the video(s) specified by `video_path`.
    The prediction results (x-/y-coordinates of the located paws) for each video
    are stored in as HDF5 and CSV in its respective folder. 

    Args:
        cfg_path (str): Path to DLC config file to load.
        video_paths (str, list[str]): Path to a video folder or list of video
            paths to run prediction on. When passing a folder, only "avi",
            "mp4", "mov", "mpeg", "mkv" are processed.
    """
    # NOTE: Importing deeplabcut and its dependencies takes some time and delays
    # command line output to appear in case the user has called the script
    # incorrectly.
    import deeplabcut

    # XXX: Config file at `cfg_path` must have snapshot index to use set.
    deeplabcut.analyze_videos(
        cfg_path, video_paths, save_as_csv=True)


def label(cfg_path: str, video_paths: Tuple[str, List[str]]):
    """
    Create a labeled video in MP4 format with labels predicted by the network.

    Args:
        cfg_path (str): Path to DLC config file to load.
        video_paths (str, list[str]): Path to a video folder or list of video
            paths to run prediction on. When passing a folder, only "avi",
            "mp4", "mov", "mpeg", "mkv" are processed.
    """
    # NOTE: Importing deeplabcut and its dependencies takes some time and delays
    # command line output to appear in case the user has called the script
    # incorrectly.
    import deeplabcut

    deeplabcut.create_labeled_video(
        cfg_path, video_paths,
        save_frames=False  # Tests show degraded quality if True.
        )


def load_config(cfg_path: str) -> Dict[str, Any]:
    """Load DLC configuration file from path `cfg_path` and return it as dict.

    Args:
        cfg_path (str): Path to DLC config file to load.
    
    Returns:
        Dict[str, Any]: 
    
    Raises:
        FileNotFoundError: if `cfg_path` is not a path to a file
    """
    path = Path(cfg_path)

    if os.path.exists(path):
        try:
            with open(path, 'r') as yml_file:
                cfg = yaml.load(yml_file, Loader=yaml.SafeLoader)
        except yaml.parser.ParserError:
            raise RuntimeError('The DLC config file is not a valid YAML file.')

    else:
        raise FileNotFoundError(
            'DLC config file not found. Make sure that the file exists '
            'and/or that you passed the path of the config file correctly!')

    return cfg


class VideoPathsAction(argparse.Action):
    """``Action`` sub-class which sets a single path (string) or a list of paths
    (strings) on the as a string or list of strings on `namespace`,
    respectively.
    """
    def __call__(self, parser: ArgumentParser, namespace: Namespace,
            values: Tuple[str, Sequence[Any], None],
            option_string: Tuple[str, None] = ...) -> None:

        if values is not None and len(values) > 0:
            if len(values) == 1:
                # Test if values is path to (video) folder.
                if Path(values[0]).is_dir():
                    setattr(namespace, self.dest, values[0])
                else:  # Assume values to be list of single video path.
                    setattr(namespace, self.dest, values)
            else:
                # Assume values to be list of video paths.
                setattr(namespace, self.dest, values)


if __name__ == '__main__':

    parser = ArgumentParser(
        prog='Inference',
        description=
            'Command-line program to analyze (predict paw locations) and label '
            'videos. See "Modes" below for details.'
    )

    subparser_name = 'mode'
    subparsers = parser.add_subparsers(
        title='Modes',
        # Set name of selected sub-command in namespace when parsing args.
        # This way, used sub-command can be detected.
        dest=subparser_name,
        description=
            'This command-line program can run in two modes: '
            '1) predict mouse paw locations in a video using a deep neural network and '
            '2) label a video for visualization purposes, based on its prediction results.'
            'Prediction results are stored as HDF5 and CSV file in the same '
            'folder where the corresponding video resides.'
    )

    sub_cmd_label = 'label'  # Name of 'label' sub command
    parser_label = subparsers.add_parser(name=sub_cmd_label, help='Label video')
    sub_cmd_predict = 'predict'  # Name of 'predict' sub command
    parser_predict = subparsers.add_parser(name=sub_cmd_predict,
        help='Predict paw locations')
    
    # NOTE: If the -v/--videos flag is to be specified before the sub command,
    # then the subcommand itself will be interpreted as a value of the flag and
    # *not* as the sub command. Therefore, move the sub command before the
    # -v/--videos flag.
    video_cmd = dict(dest='video_paths',
                     action=VideoPathsAction,
                     help=(
                         'Absolute path to a video folder or list of absolute '
                         'video paths to run prediction on. When passing a '
                         'folder, only "avi", "mp4", "mov", "mpeg", "mkv" '
                         'are processed'),
                     type=str,
                     nargs='+',  # List must not be empty
                     required=True)
    parser_label.add_argument('-v', '--videos', **video_cmd)
    parser_predict.add_argument('-v', '--videos', **video_cmd)

    cfg_cmd = dict(dest='cfg_path',
                   help='Path to DLC project config file',
                   type=str,
                   required=True)
    parser_label.add_argument('-c', '--cfg', **cfg_cmd)
    parser_predict.add_argument('-c', '--cfg', **cfg_cmd)

    #args, unknown = parser.parse_known_args()
    args = parser.parse_args()

    selected_sub_cmd = getattr(args, subparser_name)  # Is None if no sub-cmd specified
    print(f'Mode: {selected_sub_cmd}')

    load_config(args.cfg_path)  # Raises error if config not found.

    if selected_sub_cmd == sub_cmd_predict:
        predict(args.cfg_path, args.video_paths)
    elif selected_sub_cmd == sub_cmd_label:
        label(args.cfg_path, args.video_paths)
