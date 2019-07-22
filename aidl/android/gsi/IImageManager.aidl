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

package android.gsi;

import android.gsi.MappedImage;

/** {@hide} */
interface IImageManager {
    /**
     * Create an image that can be mapped as a block device.
     *
     * This call will fail if running a GSI.
     *
     * @param name          Image name. If the image already exists, the call will fail.
     * @param size          Image size, in bytes. If too large, or not enough space is
     *                      free, the call will fail.
     * @param readonly      If readonly, MapBackingImage() will configure the device as
     *                      readonly.
     * @return              True on success, false otherwise.
     */
    void createBackingImage(@utf8InCpp String name, long size, boolean readonly);

    /**
     * Delete an image created with createBackingImage.
     *
     * @param name          Image name as passed to createBackingImage().
     * @return              True on success, false otherwise.
     */
    void deleteBackingImage(@utf8InCpp String name);

    /**
     * Map an image, created with createBackingImage, such that it is accessible as a
     * block device.
     *
     * @param name          Image name as passed to createBackingImage().
     * @param timeout_ms    Time to wait for a valid mapping, in milliseconds. This must be more
     *                      than zero; 10 seconds is recommended.
     * @param mapping       Information about the newly mapped block device.
     */
    void mapImageDevice(@utf8InCpp String name, int timeout_ms, out MappedImage mapping);

    /**
     * Unmap a block device previously mapped with mapBackingImage. This step is necessary before
     * calling deleteBackingImage.
     *
     * @param name          Image name as passed to createBackingImage().
     */
    void unmapImageDevice(@utf8InCpp String name);
}