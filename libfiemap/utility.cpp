/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "utility.h"

#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/strings.h>
#include <libfiemap/fiemap_writer.h>

namespace android {
namespace fiemap {

static constexpr char kUserdataDevice[] = "/dev/block/by-name/userdata";

uint64_t DetermineMaximumFileSize(const std::string& file_path) {
    // Create the smallest file possible (one block).
    auto writer = FiemapWriter::Open(file_path, 1);
    if (!writer) {
        return 0;
    }

    uint64_t result = 0;
    switch (writer->fs_type()) {
        case EXT4_SUPER_MAGIC:
            // The minimum is 16GiB, so just report that. If we wanted we could parse the
            // superblock and figure out if 64-bit support is enabled.
            result = 17179869184ULL;
            break;
        case F2FS_SUPER_MAGIC:
            // Formula is from https://www.kernel.org/doc/Documentation/filesystems/f2fs.txt
            // 4KB * (923 + 2 * 1018 + 2 * 1018 * 1018 + 1018 * 1018 * 1018) := 3.94TB.
            result = 4329690886144ULL;
            break;
        case MSDOS_SUPER_MAGIC:
            // 4GB-1, which we want aligned to the block size.
            result = 4294967295;
            result -= (result % writer->block_size());
            break;
        default:
            LOG(ERROR) << "Unknown file system type: " << writer->fs_type();
            break;
    }

    // Close and delete the temporary file.
    writer = nullptr;
    unlink(file_path.c_str());

    return result;
}

// Given a SplitFiemap, this returns a device path that will work during first-
// stage init (i.e., its path can be found by InitRequiredDevices).
std::string GetDevicePathForFile(SplitFiemap* file) {
    auto bdev_path = file->bdev_path();

    struct stat userdata, given;
    if (!stat(bdev_path.c_str(), &given) && !stat(kUserdataDevice, &userdata)) {
        if (S_ISBLK(given.st_mode) && S_ISBLK(userdata.st_mode) &&
            given.st_rdev == userdata.st_rdev) {
            return kUserdataDevice;
        }
    }
    return bdev_path;
}

std::string JoinPaths(const std::string& dir, const std::string& file) {
    if (android::base::EndsWith(dir, "/")) {
        return dir + file;
    }
    return dir + "/" + file;
}

}  // namespace fiemap
}  // namespace android
