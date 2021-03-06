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

#ifndef INTERNAL_ERROR_H
#define INTERNAL_ERROR_H

#include "error.h"

#include <stdio.h>

typedef enum
{
    SUCCESS,
    NO_SUCH_MEMORY,
    FILE_NOT_FOUND,
    INVALID_PARAMETER,
    UNKNOWN_ERROR
} internal_error_type;

char *internal_error_get_description(internal_error_type type);

char *internal_error_to_string(error *e);

void internal_error_print(error *e, FILE *out);

#endif
