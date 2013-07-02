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
#ifndef NANDROID_H
#define NANDROID_H

int nandroid_backup(const char* backup_path);
int nandroid_advanced_backup(const char* backup_path, int boot, int recovery, int system, int data, int cache, int sdext);
int nandroid_restore(const char* backup_path, int restore_boot, int restore_system, int restore_data, int restore_cache, int restore_sdext);
int recalc_sdcard_space();
int has_datadata();
void nandroid_get_backup_path(const char* backup_path);
void nandroid_generate_timestamp_path(const char* backup_path);
void nandroid_get_assigned_backup_path(const char* backup_path);

/* These are set by recovery.c but only utilized within nandroid (currently)
 * (they may need to be moved when we import settings) */
extern const char *DEFAULT_BACKUP_PATH;
extern const char *USER_DEFINED_BACKUP_MARKER;

#endif
