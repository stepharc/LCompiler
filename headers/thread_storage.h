/*******************************************************************************
 * Copyright (C) 2019 Charly Lamothe, Stéphane Arcellier                      *
 *                                                                             *
 * This file is part of LCompiler.                                             *
 *                                                                             *
 *   Licensed under the Apache License, Version 2.0 (the "License");           *
 *   you may not use this file except in compliance with the License.          *
 *   You may obtain a copy of the License at                                   *
 *                                                                             *
 *   http://www.apache.org/licenses/LICENSE-2.0                                *
 *                                                                             *
 *   Unless required by applicable law or agreed to in writing, software       *
 *   distributed under the License is distributed on an "AS IS" BASIS,         *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *   See the License for the specific language governing permissions and       *
 *   limitations under the License.                                            *
 *******************************************************************************/

#ifndef THREAD_STORAGE_H
#define THREAD_STORAGE_H

#include "bool.h"
#include "stacktrace_struct.h"

bool thread_storage_init();

void thread_storage_uninit();

bool thread_storage_append_to_be_deleted_data(void *data);

stacktrace *thread_storage_get_stacktrace();

bool thread_storage_set_char_data(char *data);

char *thread_storage_get_char_data();

bool thread_storage_set_int_data(int data);

int thread_storage_get_int_data();

#endif
