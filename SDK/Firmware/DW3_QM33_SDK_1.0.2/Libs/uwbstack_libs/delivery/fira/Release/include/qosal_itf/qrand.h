/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-Qorvo-2
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * qrand_seed() - Initialize the seed for rand generator.
 * @seed: Initialization value.
 */
void qrand_seed(uint32_t seed);

/**
 * qrand_rand() - Return a random number.
 * Return: random. Max value is 65536.
 */
uint32_t qrand_rand(void);

#ifdef __cplusplus
}
#endif
