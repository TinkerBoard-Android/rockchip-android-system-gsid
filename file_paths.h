//
// Copyright (C) 2019 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

namespace android {
namespace gsi {

// TODO(b/122671850) change paths
static constexpr char kGsiDataFolder[] = "/data/unencrypted/gsi";
static constexpr char kUserdataFile[] = "/data/unencrypted/gsi/userdata_gsi.img";
static constexpr char kSystemFile[] = "/data/unencrypted/gsi/system_gsi.img";

static constexpr char kGsiMetadataFolder[] = "/metadata/gsi";
static constexpr char kGsiLpMetadataFile[] = "/metadata/gsi/lp_metadata";

// This file can contain the following values:
//   [int]      - boot attempt counter, starting from 0
//   "ok"       - boot was marked successful
//   "disabled" - boot into GSI no longer allowed
//   "wipe"     - boot into GSI not allowed; next reboot will delete gsi
static constexpr char kGsiInstallStatusFile[] = "/metadata/gsi/install_status";

}  // namespace gsi
}  // namespace android
