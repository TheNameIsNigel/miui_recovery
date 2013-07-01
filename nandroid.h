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

/* These really should go somewhere else but are only utilized in nandroid and
 * roots. */
int format_unknown_device(const char *device, const char* path, const char *fs_type);
int format_device(const char *device, const char *path, const char *fs_type);

#endif
