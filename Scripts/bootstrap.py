import json
import os
import subprocess
import shutil


def log(message):
    print(f"[BOOTSTRAP] {message}")


def error(message):
    log(f"ERROR: {message}")


def execute_command(command):
    log(command)
    return subprocess.call(command, shell=True, stdout=None, stderr=None)


def die(result):
    if result != 0:
        raise ValueError(f"Command returned non-zero status: {str(result)}")


def read_json_file(filename):
    try:
        json_data = open(filename).read()
    except Exception as e:
        error(f"Failed to read {filename}: {str(e)}")
        return None

    try:
        return json.loads(json_data)
    except Exception as e:
        error(f"Failed to parse JSON: {str(e)}")
        return None


def clone_repository(url, target_name, revision=None):
    target_dir = os.path.join("External/Source", target_name)
    target_dir_exists = os.path.exists(target_dir)

    log(f"Cloning {url} to {target_dir}")

    repo_exists = os.path.exists(os.path.join(target_dir, ".git"))

    if not repo_exists:
        if target_dir_exists:
            log(f"Removing directory {target_dir} before cloning")
            shutil.rmtree(target_dir)

        die(execute_command(f"git clone --recursive {url} {target_dir}"))

    if revision is None:
        revision = "HEAD"

    die(execute_command(f"git -C {target_dir} reset --hard {revision}"))
    die(execute_command(f"git -C {target_dir} clean -fxd"))


def check_vulkan_sdk():
    vulkan_sdk_path = os.environ.get("VULKAN_SDK")
    if vulkan_sdk_path:
        log(f"Vulkan SDK is available VULKAN_SDK={vulkan_sdk_path}")
        return True
    return False


def main():
    log("Starting...")

    if not check_vulkan_sdk():
        log("Vulkan SDK is not available")

    dependencies = read_json_file("Scripts/dependencies.json")

    for dependency in dependencies:
        name = dependency["name"]
        url = dependency["url"]
        revision = dependency.get("revision", None)
        clone_repository(url, name, revision)

    log("Done.")


if __name__ == "__main__":
    main()
