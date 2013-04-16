/*
 * Copyright (C) 2012 Drew Walton & Nathan Bass
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
#include <sys/reboot.h>
#include "cutils/android_reboot.h"
#include "power.h"

/* On the off chance that your device requires a non-standard command
 * to reboot or power off from recovery simply replace that command in
 * one of the following.
 *
 * It is highly unlikely you will need to change this for your device
 * this was added for the soul purpose of patching reboot in the Amazon
 * Kindle Fire which uses a binary command to reset the idme bootmode. */
void pass_normal_reboot() {
	android_reboot(ANDROID_RB_RESTART, 0, 0);
}

void pass_shutdown_cmd() {
	android_reboot(ANDROID_RB_POWEROFF, 0, 0);
}
