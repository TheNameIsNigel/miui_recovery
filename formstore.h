/**
 * Copyright (c) 2013, Project Open Cannibal
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
// Format and Storage ;)
#ifndef _FORMSTORE_H_
#define _FORMSTORE_H_

extern long tmplog_offset;

int volume_main(int argc, char **argv);
int is_path_mounted(const char* path);

int erase_volume(const char *volume);

int format_device(const char *device, const char *path, const char *fs_type);
int format_unknown_device(const char *device, const char* path, const char *fs_type);

int verify_root_and_recovery();

#endif // _FORMSTORE_H_
