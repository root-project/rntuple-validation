import json
import difflib
from pathlib import Path
from itertools import permutations
from typing import Any

class Version:
    """Class to compare version numbers in epoch.major.minor format
    """
    def __init__(self, version):
        self.parts = tuple(map(int, version.split(".")))
    
    def __eq__(self, other):
        return self.parts == other.parts
    
    def __lt__(self, other):
        return self.parts < other.parts
    
    def __le__(self, other):
        return self.parts <= other.parts
    
    def __gt__(self, other):
        return self.parts > other.parts
    
    def __ge__(self, other):
        return self.parts >= other.parts
    
    def __repr__(self):
        return f"Version({'.'.join(map(str, self.parts))})"

def load_json(path: str) -> Any:
    """Parse JSON file

    Args:
        path (str): Path to file

    Returns:
        Any: The parsed JSON content (exact type depends on JSON object (list, dict etc.)
    """

    with open(path) as f:
        return json.load(f)

def json_to_text(obj: Any) -> str:
    """Convert parsed JSON object to string sorted keys and displayed in JSON format

    Args:
        obj (Any): Parsed JSON object

    Returns:
        str: JSON object formatted as a string
    """

    return json.dumps(obj, indent=2, sort_keys=True) 

def compare_files(path_a: Path, path_b: Path) -> list:
    """Compare two JSON files.

    Args:
        path_a (string): Path to file A
        path_b (string): Path to file B

    Returns:
        list: Differences found
    """

    a = json_to_text(load_json(path_a))
    b = json_to_text(load_json(path_b))
    diff = list(difflib.unified_diff(
        b.splitlines(), a.splitlines(),
        fromfile=str(path_b), tofile=str(path_a),
        lineterm=""
    ))

    return diff

def compare_folders(folder_a: str, folder_b: str, log: callable=None) -> dict:
    """Compare files of two folders.

    `status` indicates if differences exist:
    - 0 = folders are equal
    - 1 = differences in files found
    - 2 = no files have been compared

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

    status = 0
    for name in sorted(common_files):
        diff = compare_files(folder_a / name, folder_b / name)
        if not diff:
            equal_files.append(name)
        else:
            file_diffs[name] = "\n".join(diff)

    if file_diffs: status = 1
    if not common_files: status = 2

    return {
        "folder_a": str(folder_a),
        "folder_b": str(folder_b),
        "status": status,
        "file_diffs": file_diffs,
        "missing_in_a": missing_in_a,
        "missing_in_b": missing_in_b,
        "equal_files": equal_files,
    }

def extract_versions(path: str) -> tuple[str, str]:
    """Extract write and read version of subdir path

    Args:
        path (str): Path to subdir

    Returns:
        tuple[str, str]: Write and read version of subdir
    """

    _, write_ver, read_ver = path.split("/")
    return write_ver, read_ver

def get_limiting_version(write_ver: str, read_ver: str) -> Version:
    """Determine smaller version.

    Args:
        write_ver (str): Write version
        read_ver (str): Read version

    Returns:
        Version: Smaller version
    """

    if write_ver.startswith("RNTuple"):
        return Version("0.00.00")
    return min(Version(write_ver), Version(read_ver))

def meets_comparison_logic(folder_a: str, folder_b: str) -> bool:
    """Check if the subdirectories satisfy the comparison logic.

    The subdirectory format is: <write_ver>/<read_ver>. Compare a subdir A with subdir B only if min(write_ver, read_ver) of A
    is bigger or equal to min(write_ver, read_ver) of B.

    Args:
        folder_a (str): Path to subdir A
        folder_b (str): Path to subdir B

    Returns:
        bool: True of logic is met, Flase otherwise.
    """

    write_ver_a, read_ver_a = extract_versions(folder_a)
    write_ver_b, read_ver_b = extract_versions(folder_b)

    # GitHub validation assets are only ever used as a comparison target, never compared against anything else.
    if read_ver_a.startswith("Validation"):
        return False
    if write_ver_b.startswith("RNTuple") and read_ver_b.startswith("Validation"):
        return True

    lim_ver_a = get_limiting_version(write_ver_a, read_ver_a)
    lim_ver_b = get_limiting_version(write_ver_b, read_ver_b)
    return lim_ver_a >= lim_ver_b

def cross_validation(dir: Path, log: callable=None, path: str=None) -> dict:
    """Run cross-validation of created JSON files in subdirectories of `dir`

    Args:
        dir (Path): Path to subdirectories
        log (callable, optional): Function to return loggin information. Defaults to None.
        path (str, optional): Path to store the results in a JSON file. Defaults to None.

    Raises:
        FileExistsError: Check if subdirectories were found

    Returns:
        dict: Comparison results in JSON format
    """

    sub_dirs = sorted(str(sub_dir) for sub_dir in dir.glob("*/*/"))
    if not sub_dirs:
        raise FileExistsError("No subdirectories found!")

    all_file_diffs = {}
    for folder_a, folder_b in list(permutations(sub_dirs, 2)):
        if meets_comparison_logic(folder_a, folder_b):
            all_file_diffs.setdefault(folder_a, {})[folder_b] = compare_folders(folder_a, folder_b, log)

    if path is not None:
        with open(path, "w") as f:
            json.dump(all_file_diffs, f, indent=2, sort_keys=True)

    return all_file_diffs

def fill_template(template_file, data_file, output_file):
    with open(template_file) as f:
        template_html = f.read()
    
    json_data = json_to_text(load_json(data_file))
    filled_html = template_html.replace("__PLACE_HOLDER__", json_data)

    with open(output_file, "w") as f:
        f.write(filled_html)
    
    print(f"HTML file exported to: {output_file}")

if __name__ == "__main__":
    read_dir = Path("read/")
    results = cross_validation(read_dir, path="web/cross_validation_results.json")

    # write results in HTML template
    fill_template("web/html/template.html", "web/cross_validation_results.json", "web/html/cross_validation_results.html")
