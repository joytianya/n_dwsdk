/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qmalloc.h"

#include <uwbmac/uwbmac_types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct uwbmac_msg;

/**
 * UWBMAC_BUF_CB_SIZE - Size of the control block in a network buffer structure.
 */
#define UWBMAC_BUF_CB_SIZE 16

#if defined(UWBMAC_BUF_USE_NETBUF)
#include "uwbmac/uwbmac_buf_netbuf.h"
#elif defined(UWBMAC_BUF_USE_SKB)
#include "uwbmac/uwbmac_buf_skb.h"
#else /* Fallback to generic implementation. */
#include "uwbmac/uwbmac_buf_malloc.h"
#endif

/**
 * uwbmac_buf_alloc_quota() - Allocate a new network buffer with requested size.
 * @size: Size of buffer.
 * @quota_id: Quota to use for this allocation.
 *
 * Return: Pointer to the new buffer, or NULL if no memory available.
 */
struct uwbmac_buf *uwbmac_buf_alloc_quota(unsigned int size, enum mem_quota_id quota_id);

/**
 * uwbmac_buf_alloc() - Allocate a new network buffer with requested size.
 * @size: Size of buffer.
 *
 * Return: Pointer to the new buffer, or NULL if no memory available.
 */
struct uwbmac_buf *uwbmac_buf_alloc(unsigned int size);

/**
 * uwbmac_buf_free() - Release a network buffer.
 * @buf: Buffer to release.
 */
void uwbmac_buf_free(struct uwbmac_buf *buf);

/**
 * uwbmac_buf_reserve() - Reserve some headroom on an empty buffer.
 * @buf: Buffer where space needs to be reserved, must be empty.
 * @len: Length to reserve.
 */
void uwbmac_buf_reserve(struct uwbmac_buf *buf, unsigned int len);

/**
 * uwbmac_buf_headroom() - Return available space at start of buffer.
 * @buf: Buffer.
 *
 * Return: Number of allocated free bytes before the data start.
 */
unsigned int uwbmac_buf_headroom(const struct uwbmac_buf *buf);

/**
 * uwbmac_buf_tailroom() - Return available space at end of buffer.
 * @buf: Buffer.
 *
 * Return: Number of allocated free bytes after the data end.
 */
unsigned int uwbmac_buf_tailroom(const struct uwbmac_buf *buf);

/**
 * uwbmac_buf_trim() - Trim data to the given length.
 * @buf: Buffer to trim.
 * @len: New buffer length.
 *
 * If data is smaller than the trim length, the buffer is not modified.
 *
 * NOTE: Use it only with not fragmented buffers.
 */
void uwbmac_buf_trim(struct uwbmac_buf *buf, unsigned int len);

/**
 * uwbmac_buf_put() - Prepare a buffer to append new data.
 * @buf: Buffer to prepare.
 * @len: Length of data to add.
 *
 * This function returns a pointer to the first byte where data must be written.
 * The caller must make sure that there is enough space before calling this
 * function. If fragments are used, len must not exceed the tailroom of the last
 * fragment of the buffer.
 *
 * Return: Pointer to first new byte of data.
 */
void *uwbmac_buf_put(struct uwbmac_buf *buf, unsigned int len);

/**
 * uwbmac_buf_put_data() - Append data to a buffer.
 * @buf: Buffer to write to.
 * @data: Data to append.
 * @len: Length of new data.
 *
 * The caller must make sure that there is enough space before calling this
 * function.
 *
 * Return: 0 or error.
 */
int uwbmac_buf_put_data(struct uwbmac_buf *buf, const void *data, unsigned int len);

/**
 * uwbmac_buf_put_u8() - Append a single byte to a buffer.
 * @buf: Buffer to write to.
 * @data: Single byte to append.
 *
 * The caller must make sure that there is enough space before calling this
 * function.
 */
void uwbmac_buf_put_u8(struct uwbmac_buf *buf, uint8_t data);

/**
 * uwbmac_buf_push() - Prepare a buffer to insert new data at buffer start.
 * @buf: Buffer to prepare.
 * @len: Length of new data.
 *
 * This function returns a pointer to the first byte where data must be written.
 * The caller must make sure that there is enough space before calling this
 * function.
 *
 * Return: Pointer to first new byte of data.
 */
void *uwbmac_buf_push(struct uwbmac_buf *buf, unsigned int len);

/**
 * uwbmac_buf_pull() - Extract data from the start of buffer.
 * @buf: Buffer to read.
 * @len: Length of data to extract.
 *
 * The caller must make sure that there is enough data in the buffer before
 * calling this function.
 */
void uwbmac_buf_pull(struct uwbmac_buf *buf, unsigned int len);

/**
 * uwbmac_buf_queue_init() - Initialize an empty queue.
 * @queue: Buffer queue to initialize.
 */
void uwbmac_buf_queue_init(struct uwbmac_buf_queue *queue);

/**
 * uwbmac_buf_queue_empty() - Test whether a queue is empty.
 * @queue: Buffer queue.
 *
 * Return: true if the queue is empty.
 */
bool uwbmac_buf_queue_empty(const struct uwbmac_buf_queue *queue);

/**
 * uwbmac_buf_queue_push() - Put a buffer at the start of a queue.
 * @queue: Buffer queue which will receive the buffer.
 * @buf: Buffer to insert.
 */
void uwbmac_buf_queue_push(struct uwbmac_buf_queue *queue, struct uwbmac_buf *buf);

/**
 * uwbmac_buf_queue_put() - Put a buffer at the end of a queue.
 * @queue: Buffer queue which will receive the buffer.
 * @buf: Buffer to insert.
 */
void uwbmac_buf_queue_put(struct uwbmac_buf_queue *queue, struct uwbmac_buf *buf);

/**
 * uwbmac_buf_queue_is_last() - Check if buf is the last entry in the queue.
 * @queue: Queue head.
 * @buf: Current buffer.
 *
 * Return: True if buf is the last buffer on the list.
 *
 */
bool uwbmac_buf_queue_is_last(const struct uwbmac_buf_queue *queue, const struct uwbmac_buf *buf);

/**
 * uwbmac_buf_queue_next() - Return the next packet in the queue.
 * @queue: Queue head.
 * @buf: Current buffer.
 *
 * Return: Next packet in the queue.
 */
struct uwbmac_buf *uwbmac_buf_queue_next(const struct uwbmac_buf_queue *queue,
					 const struct uwbmac_buf *buf);

/**
 * uwbmac_buf_queue_peek() - Peek a buffer from the start of a queue.
 * @queue: Buffer queue to peek the buffer from.
 *
 * Buffer is left in the queue.
 *
 * Return: The peeked buffer, or NULL if the queue is empty.
 */
struct uwbmac_buf *uwbmac_buf_queue_peek(struct uwbmac_buf_queue *queue);

/**
 * uwbmac_buf_queue_pop() - Get and remove a buffer from the start of a queue.
 * @queue: Buffer queue to extract the buffer from.
 *
 * Return: The extracted buffer, or NULL if the queue is empty.
 */
struct uwbmac_buf *uwbmac_buf_queue_pop(struct uwbmac_buf_queue *queue);

/**
 * uwbmac_buf_queue_purge() - Release all buffers in a queue.
 * @queue: Buffer queue to purge
 */
void uwbmac_buf_queue_purge(struct uwbmac_buf_queue *queue);

/**
 * uwbmac_buf_get_next_frag() - Retrieve next fragment data.
 * @buf: Buffer.
 *
 * Return: Pointer to next fragment or NULL.
 */
struct uwbmac_buf *uwbmac_buf_get_next_frag(struct uwbmac_buf *buf);

/**
 * uwbmac_buf_get_data() - Retrieve pointer to buffer data
 * @buf: Buffer.
 *
 * Return: Pointer to first byte of buffer data.
 */
uint8_t *uwbmac_buf_get_data(struct uwbmac_buf *buf);

/**
 * uwbmac_buf_get_len() - Retrieve buffer data length.
 * @buf: Buffer.
 *
 * Return: Buffer data length.
 */
unsigned int uwbmac_buf_get_len(struct uwbmac_buf *buf);

/**
 * uwbmac_buf_get_frag_len() - Retrieve current fragment data length.
 * @buf: Buffer/Fragment.
 *
 * Return: Fragment data length.
 */
unsigned int uwbmac_buf_get_frag_len(struct uwbmac_buf *buf);

/**
 * uwbmac_buf_get_size() - Retrieve buffer size.
 * @buf: Buffer.
 *
 * Return: Buffer size.
 */
unsigned int uwbmac_buf_get_size(struct uwbmac_buf *buf);

/**
 * uwbmac_buf_set_queue_mapping() - Set queue mapping field of buffer.
 * @buf: Buffer to write to.
 * @value: queue_mapping value to set.
 */
void uwbmac_buf_set_queue_mapping(struct uwbmac_buf *buf, uint16_t value);

/**
 * uwbmac_buf_free_msg_priv() - Free priv member of msg.
 * @msg: Message to free.
 */
void uwbmac_buf_free_msg_priv(struct uwbmac_msg *msg);

#ifdef __cplusplus
}
#endif
