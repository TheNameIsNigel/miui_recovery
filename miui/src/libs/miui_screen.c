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

/* Description:
 * 	Implement of screen echo or black; depends on timeout in input event or
 * 	invoke screen_set.
 * 
 * 	This file is an inner file within miui_input.c.
 * 	Static bind property on most methods; the following methods can be invoked
 * 	externally:
 * 
 * 		int screen_set_brightness(const char* brightness)
 * 		int screen_set_interval(int interval)
 * 
 * 	The following methods were already visible but there should be no need to
 * 	call them. In most cases use the above:
 * 
 * 		int screen_set_black(int black)
 * 		int screen_is_black()
 * 		int screen_set_light(int light)
 * 
 * 	Original Author: Dennise
 * 	email: yanhao@xiaomi.com
 * 
 * 	Last Modified by: Nathan Bass
 * 	email: sblood86@gmail.com
 */
#include <errno.h>
#include "miui_screen.h"

static time_t time_orig;
//for further consideration, change time_interval in device.conf
static time_t time_interval;
static int bool_black = 0;
static pthread_mutex_t mutex_screen = PTHREAD_MUTEX_INITIALIZER;
//screen_ligt, [0-255] for config by device_config
static char screen_light_buf[4];
// Stop flooding my recovery log!!!
static int invalid_brightness_path = 0;

static int screen_set_time(time_t time) {
	return_val_if_fail(time > 0, -1);
	miui_debug("set time %ld\n", time);
	time_orig = time;
	return 0;
}

int screen_set_brightness(const char* brightness)
{
	pthread_mutex_lock(&mutex_screen);
	/* Instead of flooding the recovery log check a status and only run this the
	 * one time anyway. */
	if(invalid_brightness_path != 0) {
		/* Return 0 so's to not error (I'll be changing this to provide proper
		 * error handling in the future). */
		return 0;
	}
	int fd = open(acfg()->brightness_path, O_WRONLY);
	// Just for good measure check our status again before trying to change it.
	if(fd <= 0 && invalid_brightness_path == 0) {
		miui_error("open %s failed!\n", acfg()->brightness_path);
		invalid_brightness_path = 1;
	} else {
		if (write(fd, brightness, 1) <= 0) {
			miui_error("% write error %s", acfg()->brightness_path, strerror(errno));
		}
		close(fd);
	}
	pthread_mutex_unlock(&mutex_screen);
	return 0;
}

int screen_set_black(int black) {
	pthread_mutex_lock(&mutex_screen);
	if(invalid_brightness_path != 0) {
		return 0;
	}
	int fd = open(acfg()->brightness_path, O_WRONLY);
	if(fd <= 0 && invalid_brightness_path == 0) {
		miui_error("open %s failed!\n", acfg()->brightness_path);
		invalid_brightness_path = 1;
	} else {
		bool_black = black;
		if(bool_black) {
			if(write(fd,"0", 1) <= 0) {
				miui_error("%s write error %s", acfg()->brightness_path, strerror(errno));
			}
		} else {
			miui_debug("screen_light_buf is %s\n", screen_light_buf);
			if(write(fd,screen_light_buf,strlen(screen_light_buf)) <= 0) {
				miui_error("%s write error %s", acfg()->brightness_path, strerror(errno));
			}
		}
		close(fd);
	}
	pthread_mutex_unlock(&mutex_screen);
	return 0;
}

int screen_is_black() {
	return bool_black;
}

int screen_set_light(int light) {
	return_val_if_fail(light > 10, -1);
	return_val_if_fail(light <= 255, -1);
	return_val_if_fail(0 <= snprintf(screen_light_buf,sizeof(screen_light_buf), "%d", light), -1);
	return 0;
}

int screen_set_interval(int interval) {
	return_val_if_fail(interval > 0, -1);
	time_interval = interval;
	return 0;
}

static void *screen_black_thread(void *cookie) {
	while(1) {
		if(difftime(time((time_t *)NULL), time_orig) > time_interval) {
			if(0 == bool_black) {
				screen_set_black(1);
			}
		} else if (0 != bool_black) {
			screen_set_black(0);
		}
		sleep(1);
	}
	return NULL;
}

static pthread_t screen_thread_t;
static int screen_init() {
	//default interval 120 seconds
	time_interval = 120;
	screen_set_light(60);
	screen_set_time(time((time_t*)NULL));
	screen_set_black(0);

	pthread_create(&screen_thread_t, NULL, screen_black_thread, NULL);
	pthread_detach(screen_thread_t);
	return 0;
}
