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
#ifndef _UI_SCREEN_H_
#define _UI_SCREEN_H_

int screen_set_brightness(int brightness);
int screen_set_interval(int interval);

/* The following methods were already visible in the C file but should not be
 * needed. In most cases use the above. */
int screen_set_black(int black);
int screen_is_black();
int screen_set_light(int light);

#endif // _UI_SCREEN_H_
