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

#include "../headers/internal_error.h"

#include <stdlib.h>
#include <string.h>

char *internal_error_get_description(internal_error_type type) {
    size_t size;
    char *description;

    size = 50;
    description = (char *)malloc(size * sizeof(char));

    switch (type) {
        case SUCCESS:
            strncpy(description, "No error detected", size);
        break;

        case FILE_NOT_FOUND:
            strncpy(description, "File not found", size);
        break;

        case NO_SUCH_MEMORY:
            strncpy(description, "No such memory available", size);
        break;

        case INVALID_PARAMETER:
            strncpy(description, "Specified parameter is invalid", size);
        break;

        case UNKNOWN_ERROR:
            strncpy(description, "Unknown error", size);
        break;

        default:
            strncpy(description, "Unknown error type", size);
    }

    description = (char *)realloc(description, (strlen(description) + 1) * sizeof(char));

    return description;
}

char *internal_error_to_string(error *e) {
    size_t size;
    char *error_buffer, *line_number_buffer, *prefix;

    error_buffer = NULL;
    line_number_buffer = NULL;
    prefix = NULL;

    line_number_buffer = (char *)malloc(10 * sizeof(char));
    sprintf(line_number_buffer, "%d", e->line_number);

    size = strlen("   at  (:)");
    size += strlen(e->func_name);
    size += strlen(e->file_name);
    size += strlen(line_number_buffer);
    free((void*)line_number_buffer);

    if (e->is_main_error) {
        size += strlen("Caused by: \n");
        size += strlen(e->description);
        prefix = (char *)malloc((strlen("Caused by: \n") + strlen(e->description) + 1) * sizeof(char));
        sprintf(prefix, "Caused by: %s\n", e->description);
    } else {
        prefix = (char *)malloc(sizeof(char));
        strcpy(prefix, "");
    }

    error_buffer = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        error_buffer, 
        "%s   at %s (%s:%d)",
        prefix,
        e->func_name,
        e->file_name, 
        e->line_number
    );

    free((void*)prefix);

    return error_buffer;
}

void internal_error_print(error *e, FILE *out) {
    char *error_buffer;

    error_buffer = internal_error_to_string(e);
    
    fprintf(out, "%s\n", error_buffer);

    free((void*)error_buffer);
}
