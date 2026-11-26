# This script can be run with an installed verovio package or from a CMake
# build directory such as ./bindings/python.
import argparse
import json
import multiprocessing
import os
import sys
import xml.etree.ElementTree as ET
from concurrent.futures import ProcessPoolExecutor
from importlib import resources
from pathlib import Path

from cairosvg import svg2png

# Add path for toolkit built in-place
sys.path.append('./verovio')
import verovio

# Optional list for processing only listed test files:
# files must be listed in a file passed with --shortlist, one per line (e.g. 'accid/accid-001.mei').
MEI_NS = 'http://www.music-encoding.org/ns/mei'
MEI_HEAD_TAG = f'{{{MEI_NS}}}meiHead'
EXT_META_TAG = f'{{{MEI_NS}}}extMeta'

TEST_OPTIONS = {
    'adjustPageHeight': True,
    'breaks': 'auto',
    'pageHeight': 2970,
    'pageWidth': 2100,
    'header': 'none',
    'footer': 'none',
    'scale': 40,
    'spacingStaff': 4,
    'xmlIdChecksum': True,
}

_worker_tk = None


def _discover_resource_path() -> str:
    source_tree_data = Path(__file__).resolve().parent.parent / 'data'
    package_data = resources.files(verovio) / 'data'

    # 1. Allow setting the resource path directly
    if p := os.environ.get('VEROVIO_RESOURCE_PATH'):
        return p
    # 2. Try to auto-discover it from the package
    if package_data.is_dir():
        return str(package_data)
    # 3. Try to discover it from the external source tree
    if source_tree_data.is_dir():
        return str(source_tree_data)

    print('Could not discover resources path. Exiting.')
    sys.exit(-1)


def _init_worker(resource_path: str) -> None:
    global _worker_tk
    _worker_tk = verovio.toolkit(False)
    _worker_tk.setResourcePath(resource_path)


def _read_mei_options(mei_file: Path) -> dict:
    for _, elem in ET.iterparse(mei_file, events=('end',)):
        if elem.tag == EXT_META_TAG and elem.text and elem.text.strip():
            return json.loads(elem.text)
        if elem.tag == MEI_HEAD_TAG:
            break
        elem.clear()
    return {}


def _render_file(task: tuple[Path, Path]) -> str:
    input_file, out_dir = task
    tk = _worker_tk
    assert tk is not None

    options = TEST_OPTIONS
    if input_file.suffix == '.mei':
        if meta_options := _read_mei_options(input_file):
            options = TEST_OPTIONS | meta_options

    tk.setOptions(options)
    tk.loadFile(str(input_file))

    svg_string = tk.renderToSVG(1)
    stem = input_file.stem
    (out_dir / f'{stem}.svg').write_text(svg_string, encoding='utf-8')
    svg2png(bytestring=svg_string, scale=2, write_to=str(out_dir / f'{stem}.png'))
    tk.renderToTimemapFile(str(out_dir / f'{stem}.json'))
    tk.renderToMIDIFile(str(out_dir / f'{stem}.mid'))
    tk.resetOptions()
    return input_file.name


def _collect_input_files(test_suite_dir: Path, shortlist_path: str) -> list[Path]:
    if shortlist_path:
        print(shortlist_path)
        shortlist = {
            line.strip()
            for line in Path(shortlist_path).read_text(encoding='utf-8').splitlines()
            if line.strip()
        }
        for entry in sorted(shortlist):
            print(f'File {entry} added to the shortlist')
        return sorted(p for rel in shortlist if (p := test_suite_dir / rel).is_file())

    # The round-trip runner renders musicxml-export fixtures before and after MusicXML conversion.
    return sorted(
        p
        for p in test_suite_dir.glob('*/*')
        if p.is_file()
        and not p.name.startswith('.')
        and not p.parent.name.startswith('.')
        and p.parent.name != 'musicxml-export'
    )


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('test_suite_dir')
    parser.add_argument('output_dir')
    parser.add_argument('--shortlist', nargs='?', default='')
    parser.add_argument('-j', '--jobs', type=int, default=None, help='Number of worker processes')
    args = parser.parse_args()

    tk = verovio.toolkit(False)
    print(f'Verovio {tk.getVersion()}')

    resource_path = _discover_resource_path()
    print(f'Setting resource path to {resource_path}')

    test_suite_dir = Path(args.test_suite_dir.replace(r'\ ', ' '))
    output_dir = Path(args.output_dir.replace(r'\ ', ' '))
    input_files = _collect_input_files(test_suite_dir, args.shortlist)

    for subdir in {p.parent.name for p in input_files}:
        (output_dir / subdir).mkdir(parents=True, exist_ok=True)

    tasks = [(p, output_dir / p.parent.name) for p in input_files]
    workers = args.jobs if args.jobs is not None else (os.cpu_count() or 1)

    if workers <= 1 or len(tasks) <= 1:
        _worker_tk = tk
        _worker_tk.setResourcePath(resource_path)
        for task in tasks:
            print(f'Rendering {_render_file(task)}')
    else:
        chunksize = max(1, min(8, len(tasks) // (workers * 2) or 1))
        with ProcessPoolExecutor(
            max_workers=workers,
            mp_context=multiprocessing.get_context('spawn'),
            initializer=_init_worker,
            initargs=(resource_path,),
        ) as executor:
            for filename in executor.map(_render_file, tasks, chunksize=chunksize):
                print(f'Rendering {filename}')
