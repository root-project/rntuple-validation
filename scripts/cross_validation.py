import json
import difflib
from pathlib import Path
from itertools import permutations

def load_json(path):
    with open(path) as f: return json.load(f)

def json_to_text(obj):
    return json.dumps(obj, indent=2, sort_keys=True) # convert JSON list to string with sorted keys and displayed in JSON format

def compare_files(path_a, path_b):
    """Compare two JSON files.

    Args:
        path_a (string): Path to file A
        path_b (string): Path to file B

    Returns:
        list: Differences found
    """
    # compare JSON files
    a = json_to_text(load_json(path_a))
    b = json_to_text(load_json(path_b))
    diff = list(difflib.unified_diff(
        a.splitlines(), b.splitlines(),
        fromfile=str(path_a), tofile=str(path_b),
        lineterm=""
    ))
    return diff


def compare_folders(folder_a, folder_b, log=None):
    """Compare files of two folders.
    `status` indicates if differences exist:
    - 0 = folders are equal
    - 1 = differences in files found
    - 2 = differences in file structure (different namens, num of files etc.) found

    Args:
        folder_a (string): Path to folder A
        folder_b (string): Path to folder B
        log (callable, optional): Function to log information. Defaults to None.

    Returns:
        dict: Summary of the comparison with differences in files and folder structure
    """
    if log is not None:
        log(f"Comparing '{str(folder_a)}' with '{str(folder_b)}'")

    folder_a, folder_b = Path(folder_a), Path(folder_b)
    files_a = {f.name for f in folder_a.glob("*.json")}
    files_b = {f.name for f in folder_b.glob("*.json")}

    common_files = files_a & files_b
    missing_in_a = sorted(files_b - files_a)
    missing_in_b = sorted(files_a - files_b)

    file_diffs = {}
    equal_files = []

    for name in sorted(common_files):
        diff = compare_files(folder_a / name, folder_b / name)
        if diff:
            file_diffs[name] = "\n".join(diff)
        else:
            equal_files.append(name)

    # file diff has higher priority than missing files
    status = 0
    if missing_in_a or missing_in_b: status = 2
    if file_diffs: status = 1

    return {
        "folder_a": str(folder_a),
        "folder_b": str(folder_b),
        "status": status,
        "file_diffs": file_diffs,
        "missing_in_a": missing_in_a,
        "missing_in_b": missing_in_b,
        "equal_files": equal_files,
    }

def cross_validation(dir, log=None, path=None):
    # compare each subdir with the next ones
    sub_dirs = sorted([str(sub_dir) for sub_dir in dir.glob("*/*/")])

    # if empty
    if not sub_dirs:
        raise FileExistsError("No subdirectories found!")
    
    all_file_diffs = {}
    for folder_a, folder_b in list(permutations(sub_dirs, 2)):
        all_file_diffs.setdefault(folder_a, {})[folder_b] = compare_folders(folder_a, folder_b, log)

    if path is not None:
        with open(path, "w") as f:
            json.dump(all_file_diffs, f, indent=2, sort_keys=True)

    return all_file_diffs

if __name__ == "__main__":
    read_dir = Path("read/")
    results = cross_validation(read_dir, path="test_results.json")
