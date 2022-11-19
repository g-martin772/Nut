import os
import subprocess
import CheckPython

# Make sure everything we need is installed
CheckPython.ValidatePackages()

import Vulkan

# Change from Scripts directory to root
os.chdir('../')

if not os.path.isdir("Nut/vendor/VulkanSDK"):
    os.makedirs("Nut/vendor/VulkanSDK")

if (not Vulkan.CheckVulkanSDK()):
    print("Vulkan SDK not installed.")

# if (not Vulkan.CheckVulkanSDKDebugLibs()):
#     print("Vulkan SDK debug libs not found.")

print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

print("Running premake...")
subprocess.call(["vendor/premake/bin/premake5.exe", "vs2022"])