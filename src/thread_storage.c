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

#include "../headers/thread_storage.h"
#include "../headers/stacktrace.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    long thread_id;
    stacktrace *st;
    void **to_be_deleted;
    int to_be_deleted_number;
    char *char_data;
    int int_data;
} thread_data;

typedef struct {
    thread_data **data;
    int data_number;
} thread_storage;

thread_storage *storage = NULL;

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include <pthread.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define get_current_thread_id() GetCurrentThread()
#else
    #define get_current_thread_id() pthread_self()
#endif

static thread_data *resolve_current_thread_data() {
    int i;
    long current_thread_id;

    if (!storage || !storage->data) {
        return NULL;
    }

    current_thread_id = get_current_thread_id();
    if (!current_thread_id) {
        return NULL;
    }

    for (i = 0; i < storage->data_number; i++) {
        if (storage->data[i]->thread_id == current_thread_id) {
            return storage->data[i];
        }
    }

    for (i = 0; i < storage->data_number; i++) {
        if (storage->data[i]->thread_id == -1) {
            storage->data[i]->thread_id = current_thread_id;
            stacktrace_create(&storage->data[i]->st);
            return storage->data[i];
        }
    }

    return NULL;
}

static void thread_data_destroy(thread_data *td) {
    int i;

    if (!td) {
        return;
    }

    stacktrace_destroy(td->st);

    if (td->to_be_deleted) {
        for (i = 0; i < td->to_be_deleted_number; i++) {
            free((void *)td->to_be_deleted[i]);
            td->to_be_deleted[i] = NULL;
        }
        free((void *)td->to_be_deleted);
        td->to_be_deleted = NULL;
    }

    free((void *)td);
}

bool thread_storage_init() {
    int i;

    storage = (thread_storage *)malloc(sizeof(thread_storage));
    memset(storage, 0, sizeof(thread_storage));

    storage->data = (thread_data **)malloc(10 * sizeof(thread_data *));
    memset(storage->data, 0, 10 * sizeof(thread_data *));
    storage->data_number = 10;
    for (i = 0; i < 10; i++) {
        storage->data[i] = (thread_data *)malloc(sizeof(thread_data));
        memset(storage->data[i], 0, sizeof(thread_data));
        storage->data[i]->thread_id = -1;
    }
}

void thread_storage_uninit() {
    int i, j;

    if (storage) {
        if (storage->data) {
            for (i = 0; i < storage->data_number; i++) {
                thread_data_destroy(storage->data[i]);
            }
            free((void *)storage->data);
        }
        free((void *)storage);
    }
}

bool thread_storage_append_to_be_deleted_data(void *data) {
    thread_data *current_thread_data;

    if (!data) {
        return false;
    }

    current_thread_data = resolve_current_thread_data();
    if (!current_thread_data) {
        return false;
    }

    if (current_thread_data->to_be_deleted) {
        current_thread_data->to_be_deleted = 
            (void **)realloc(
                current_thread_data->to_be_deleted, 
                (current_thread_data->to_be_deleted_number + 1) * sizeof(void *)
            );        
    } else {
        current_thread_data->to_be_deleted = (void **)malloc(sizeof(void *));
    }

    current_thread_data->to_be_deleted[current_thread_data->to_be_deleted_number] = data;

    current_thread_data->to_be_deleted_number++;

    return true;
}

stacktrace *thread_storage_get_stacktrace() {
    thread_data *current_thread_data;

    current_thread_data = resolve_current_thread_data();
    if (!current_thread_data) {
        return NULL;
    }

    return current_thread_data->st;
}

bool thread_storage_set_char_data(char *data) {
    thread_data *current_thread_data;

    if (!data) {
        return false;
    }

    current_thread_data = resolve_current_thread_data();
    if (!current_thread_data) {
        return false;
    }

    current_thread_data->char_data = data;

    return true;
}

char *thread_storage_get_char_data() {
    thread_data *current_thread_data;

    current_thread_data = resolve_current_thread_data();
    if (!current_thread_data) {
        return NULL;
    }

    return current_thread_data->char_data;
}

bool thread_storage_set_int_data(int data) {
    thread_data *current_thread_data;

    if (!data) {
        return false;
    }

    current_thread_data = resolve_current_thread_data();
    if (!current_thread_data) {
        return false;
    }

    current_thread_data->int_data = data;

    return true;
}

int thread_storage_get_int_data() {
    thread_data *current_thread_data;

    current_thread_data = resolve_current_thread_data();
    if (!current_thread_data) {
        return -1;
    }

    return current_thread_data->int_data;
}
