#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "../miui_inter.h"
#include "../miui.h"
#include "../../../miui_intent.h"
#include "../libs/miui_screen.h"

static STATUS nothing(struct _menuUnit *p)
{
    return MENU_BACK;
}

static STATUS brightness_menu_show(struct _menuUnit *p)
{
	switch(p->result) {
		case 25:
			screen_set_brightness("25");
			break;
		case 50:
			screen_set_brightness("50");
			break;
		case 75:
			screen_set_brightness("75");
			break;
		case 100:
			screen_set_brightness("99");
			break;
		default:
			//we should never get here!
			break;
	}
	return MENU_BACK;
}

static STATUS forcereboot_menu_show(struct _menuUnit* p)
{
    // TODO: detect current status (for now hard-code as disabled)
    int currstatus = 0;
    char *statusdlg;
    
    if (currstatus == 0)
        statusdlg = "Forced reboots are currently disabled.";
    else
        statusdlg = "Forced reboots are currently enabled.";
    
    if (RET_YES == miui_confirm(5, p->name, statusdlg, p->icon, "Enable", "Disable")) {
        // do nothing here
    }
    return MENU_BACK;
}

static STATUS wipeprompt_menu_show(struct _menuUnit* p)
{
    // TODO: detect current status (for now hard-code as disabled)
    int currstatus = 0;
    char *statusdlg;
    
    if (currstatus == 0)
        statusdlg = "Wipe prompts are currently disabled.";
    else
        statusdlg = "Wipe prompts are currently enabled.";
    
    if (RET_YES == miui_confirm(5, p->name, statusdlg, p->icon, "Enable", "Disable")) {
        // do nothing here
    }
    return MENU_BACK;
}

static STATUS backupprompt_menu_show(struct _menuUnit* p)
{
    // TODO: detect current status (for now hard-code as disabled)
    int currstatus = 0;
    char *statusdlg;
    
    if (currstatus == 0)
        statusdlg = "Backup prompts are currently disabled.";
    else
        statusdlg = "Backup prompts are currently enabled.";
    
    if (RET_YES == miui_confirm(5, p->name, statusdlg, p->icon, "Enable", "Disable")) {
        // do nothing here
    }
    return MENU_BACK;
}

static STATUS sigcheck_menu_show(struct _menuUnit* p)
{
    // TODO: detect current status (for now hard-code as disabled)
    int currstatus = 0;
    char *statusdlg;
    
    if (currstatus == 0)
        statusdlg = "Signature checks are currently disabled.";
    else
        statusdlg = "Signature checks are currently enabled.";
    
    if (RET_YES == miui_confirm(5, p->name, statusdlg, p->icon, "Enable", "Disable")) {
        // do nothing here
    }
    return MENU_BACK;
}

static STATUS battary_menu_show(struct _menuUnit* p)
{
    if (RET_YES == miui_confirm(3, p->name, p->desc, p->icon)) {
        miuiIntent_send(INTENT_MOUNT, 1, "/data");
        unlink("/data/system/batterystats.bin");
        miuiIntent_send(INTENT_UNMOUNT, 1, "/data");
        miui_printf("Battery Stats wiped.\n");
    }
    return MENU_BACK;
}
static STATUS permission_menu_show(struct _menuUnit* p)
{
    miuiIntent_send(INTENT_MOUNT, 1, "/system");
    miuiIntent_send(INTENT_MOUNT, 1, "/data");
    miuiIntent_send(INTENT_SYSTEM, 1, "fix_permissions");
    miui_alert(4, p->name, "<~global_done>", "@alert", acfg()->text_ok);
    return MENU_BACK;
}
static STATUS log_menu_show(struct _menuUnit* p)
{
    char desc[512];
    char file_name[PATH_MAX];
    struct stat st;
    time_t timep;
    struct tm *time_tm;
    time(&timep);
    time_tm = gmtime(&timep);
    if (stat(RECOVERY_PATH, &st) != 0)
    {
        mkdir(RECOVERY_PATH, 0755);
    }
    snprintf(file_name, PATH_MAX - 1, "%s/log", RECOVERY_PATH);
    if (stat(file_name, &st) != 0)
    {
        mkdir(file_name, 0755);
    }
    snprintf(file_name, PATH_MAX - 1, "%s/log/recovery-%02d%02d%02d-%02d%02d.log", RECOVERY_PATH,
           time_tm->tm_year, time_tm->tm_mon + 1, time_tm->tm_mday,
           time_tm->tm_hour, time_tm->tm_min);
    snprintf(desc, 511, "%s%s?",p->desc, file_name);
    if (RET_YES == miui_confirm(3, p->name, desc, p->icon)) {
        miuiIntent_send(INTENT_MOUNT, 1, "/sdcard");
        miuiIntent_send(INTENT_COPY, 2, MIUI_LOG_FILE, file_name);
    }
    return MENU_BACK;
}

struct _menuUnit* theme_ui_init()
{
    struct _menuUnit *p = common_ui_init();
    return_null_if_fail(p != NULL);
    menuUnit_set_name(p, "Theme");
    menuUnit_set_title(p, "Theme");
    menuUnit_set_icon(p, "@tool");
    assert_if_fail(menuNode_init(p) != NULL);
    struct _menuUnit *temp = common_ui_init();
    menuUnit_set_name(temp, "Cannibal Ice");
    menuUnit_set_show(temp, &nothing);
    menuUnit_set_result(temp, "ice");
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    return p;
}

struct _menuUnit* brightness_ui_init()
{
	struct _menuUnit *p = common_ui_init();
	return_null_if_fail(p != NULL);
	menuUnit_set_name(p, "Brightness");
	menuUnit_set_title(p, "Brightness");
	menuUnit_set_icon(p, "@tool");
	assert_if_fail(menuNode_init(p) != NULL);
	//25% brightness
	struct _menuUnit *temp = common_ui_init();
	menuUnit_set_name(temp, "25% Brightness");
	menuUnit_set_show(temp, &brightness_menu_show);
	menuUnit_set_result(temp, 25);
	assert_if_fail(menuNode_add(p, temp) == RET_OK);
	//50% brightness
	temp = common_ui_init();
	menuUnit_set_name(temp, "50% Brightness");
	menuUnit_set_show(temp, &brightness_menu_show);
	menuUnit_set_result(temp, 50);
	assert_if_fail(menuNode_add(p, temp) == RET_OK);
	//75% brightness
	temp = common_ui_init();
	menuUnit_set_name(temp, "75% Brightness");
	menuUnit_set_show(temp, &brightness_menu_show);
	menuUnit_set_result(temp, 75);
	assert_if_fail(menuNode_add(p, temp) == RET_OK);
	//100% brightness
	temp = common_ui_init();
	menuUnit_set_name(temp, "100% Brightness");
	menuUnit_set_show(temp, &brightness_menu_show);
	menuUnit_set_result(temp, 100);
	assert_if_fail(menuNode_add(p, temp) == RET_OK);
	return p;
}

/* ORS SETTINGS BEGIN */
struct _menuUnit* ors_ui_init()
{
    struct _menuUnit *p = common_ui_init();
    return_null_if_fail(p != NULL);
    menuUnit_set_name(p, "OpenRecoveryScript");
    menuUnit_set_title(p, "OpenRecoveryScript");
    menuUnit_set_icon(p, "@tool");
    assert_if_fail(menuNode_init(p) != NULL);
    
    struct _menuUnit *temp = common_ui_init();
    menuUnit_set_name(temp, "Force Reboots"); 
    menuUnit_set_icon(temp, "@tool");
    menuUnit_set_show(temp, &forcereboot_menu_show);
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    
    temp = common_ui_init();
    menuUnit_set_name(temp, "Wipe Prompt"); 
    menuUnit_set_icon(temp, "@tool");
    menuUnit_set_show(temp, &wipeprompt_menu_show);
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    return p;
}
/* ORS SETTINGS END */

/* NANDROID SETTINGS BEGIN */
struct _menuUnit* nand_ui_init()
{
    struct _menuUnit *p = common_ui_init();
    return_null_if_fail(p != NULL);
    menuUnit_set_name(p, "Nandroid");
    menuUnit_set_title(p, "Nandroid");
    menuUnit_set_icon(p, "@tool");
    assert_if_fail(menuNode_init(p) != NULL);
    
    struct _menuUnit *temp = common_ui_init();
    menuUnit_set_name(temp, "Backup Prompt"); 
    menuUnit_set_icon(temp, "@tool");
    menuUnit_set_show(temp, &backupprompt_menu_show);
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    
    temp = common_ui_init();
    menuUnit_set_name(temp, "Signature Check"); 
    menuUnit_set_icon(temp, "@tool");
    menuUnit_set_show(temp, &sigcheck_menu_show);
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    return p;
}
/* NANDROID SETTINGS END */

/* PREFS MENU BEGIN */
struct _menuUnit* prefs_ui_init()
{
    struct _menuUnit *p = common_ui_init();
    return_null_if_fail(p != NULL);
    menuUnit_set_name(p, "Preferences");
    menuUnit_set_title(p, "Preferences");
    menuUnit_set_icon(p, "@tool");
    assert_if_fail(menuNode_init(p) != NULL);
    // theme
    struct _menuUnit *temp = theme_ui_init();
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    //set brightness
    temp = brightness_ui_init();
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    // ors settings
    temp = ors_ui_init();
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    // nandroid settings
    temp = nand_ui_init();
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    return p;
}
/* PREFS MENU END */

struct _menuUnit* tool_ui_init()
{
    struct _menuUnit *p = common_ui_init();
    return_null_if_fail(p != NULL);
    menuUnit_set_name(p, "<~tool.name>");
    menuUnit_set_title(p, "<~tool.title>");
    menuUnit_set_icon(p, "@tool");
    assert_if_fail(menuNode_init(p) != NULL);
    //battery wipe
    struct _menuUnit *temp = common_ui_init();
    menuUnit_set_name(temp, "<~tool.battary.name>"); 
    menuUnit_set_icon(temp, "@tool.battery");
    menuUnit_set_show(temp, &battary_menu_show);
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    //copy log
    temp = common_ui_init();
    menuUnit_set_name(temp, "<~tool.log.name>"); 
    menuUnit_set_show(temp, &log_menu_show);
    menuUnit_set_icon(temp, "@tool.log");
    menuUnit_set_desc(temp, "<~tool.log.desc>");
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    //fix permission
    temp = common_ui_init();
    menuUnit_set_name(temp, "<~tool.permission.name>"); 
    menuUnit_set_icon(temp, "@tool.permission");
    menuUnit_set_show(temp, &permission_menu_show);
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    //prefs
    temp = prefs_ui_init();
    assert_if_fail(menuNode_add(p, temp) == RET_OK);
    return p;
}
