# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

cmake_minimum_required(VERSION 3.19)

include(${MY_TARGET_SCRIPT})

project(${MY_TARGET})

include(${PROJECT_COMMON}/cmakefiles/${MY_PROJECT}-${MY_OS}.cmake)
