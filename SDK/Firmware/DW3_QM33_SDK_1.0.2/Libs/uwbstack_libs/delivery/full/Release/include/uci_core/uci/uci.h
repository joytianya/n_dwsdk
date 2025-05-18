/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <qerr.h>
#include <qmutex.h>
#include <uci/uci_compat.h>
#include <uci/uci_spec_fira.h>
#include <uci/uci_spec_qorvo.h>

/**
 * DOC: Overview
 *
 * The UCI core implements packets parsing, packets generation, error
 * processing, segmentation, reassembly and routing. It can be used to implement
 * a UCI server (which receives commands) or UCI client (which receives
 * responses and notifications).
 *
 * The packets are sent and received using a transport channel which is
 * responsible for framing, and communication with the transport driver.
 *
 * Received commands are given to message handlers which decode the payload.
 */

/**
 * DOC: Memory handling
 *
 * To allow zero copy, buffers used to store packet data are allocated
 * dynamically. This can be backed with simple heap allocation, or using
 * a limited pool of memory blocks.
 *
 * To limit memory fragmentation, packet buffers can be split in smaller
 * blocks. This is decided by the memory allocator. In this case, a packet can
 * be composed of several blocks which are chained together. The first block
 * contains the information of the full packet length.
 */

/*
 * UCI_BLK_SIZE_MIN - Minimal size of a buffer block.
 *
 * The minimal size of a block is 20 bytes. This is needed for segmentation of
 * a block over several UCI packets, 4 bytes for the header, and 16 bytes for
 * a block descriptor.
 *
 * This minimal size ought to be respected by allocator
 * unless provided size_hint specified less.
 */
#define UCI_BLK_SIZE_MIN 20

/**
 * UCI_MAX_CONTROL_PAYLOAD_SIZE - Maximum size of a UCI Control Packet payload.
 */
#define UCI_MAX_CONTROL_PAYLOAD_SIZE 255

/**
 * UCI_MAX_DATA_PAYLOAD_SIZE - Maximum size of a UCI Data Packet payload.
 *
 * In theory the size can go up to 65535. It could be supported optionally.
 */
#define UCI_MAX_DATA_PAYLOAD_SIZE 255

/**
 * UCI_DATA_MESSAGE_SND_HEADER - Size of the DATA_MESSAGE_SND header.
 *
 * All the other fields besides Application Data.
 */
#define UCI_DATA_MESSAGE_SND_HEADER 16

/**
 * UCI_MAX_APPLICATION_DATA_PAYLOAD_SIZE_IN_FIRST_SEGMENT - Maximum size of
 * application data payload that can fit in the first Data Packet.
 */
#define UCI_MAX_APPLICATION_DATA_PAYLOAD_SIZE_IN_FIRST_SEGMENT \
	(UCI_MAX_DATA_PAYLOAD_SIZE - UCI_DATA_MESSAGE_SND_HEADER)

/**
 * UCI_MAX_APPLICATION_DATA_PAYLOAD_SIZE_IN_SEGMENT - Maximum size of
 * application data payload that can fit in one Data Packet.
 */
#define UCI_MAX_APPLICATION_DATA_PAYLOAD_SIZE_IN_SEGMENT (UCI_MAX_DATA_PAYLOAD_SIZE)

/**
 * UCI_MAX_SE_TESTING_PAYLOAD_SIZE - Maximum size of a UCI SE Testing Packet
 * payload.
 *
 * TODO: In theory the size can go up to 65535.
 * Once transport are supporting UWB-7403 we may want to rise this value.
 */
#define UCI_MAX_SE_TESTING_PAYLOAD_SIZE 255

/**
 * UCI_MAX_PAYLOAD_SIZE - Maximum size of a UCI payload.
 *
 * Should be set to max(UCI_MAX_CONTROL_PAYLOAD_SIZE, UCI_MAX_DATA_PAYLOAD_SIZE,
 * UCI_MAX_SE_TESTING_PAYLOAD_SIZE)
 */
#define UCI_MAX_PAYLOAD_SIZE (UCI_MAX_CONTROL_PAYLOAD_SIZE)

/**
 * UCI_PACKET_HEADER_SIZE - Size of a UCI packet header.
 *
 * Note: this also the minimum packet size.
 */
#define UCI_PACKET_HEADER_SIZE 4

/**
 * UCI_MAX_CONTROL_PACKET_SIZE - Maximum size of a UCI Control Packet.
 */
#define UCI_MAX_CONTROL_PACKET_SIZE (UCI_PACKET_HEADER_SIZE + UCI_MAX_CONTROL_PAYLOAD_SIZE)

/**
 * UCI_MAX_DATA_PACKET_SIZE - Maximum size of a UCI Control Packet.
 */
#define UCI_MAX_DATA_PACKET_SIZE (UCI_PACKET_HEADER_SIZE + UCI_MAX_DATA_PAYLOAD_SIZE)

/**
 * UCI_MAX_SE_TESTING_PACKET_SIZE - Maximum size of a UCI SE Testing Packet.
 */
#define UCI_MAX_SE_TESTING_PACKET_SIZE (UCI_PACKET_HEADER_SIZE + UCI_MAX_SE_TESTING_PAYLOAD_SIZE)

/**
 * UCI_MAX_PACKET_SIZE - Maximum size of a UCI packet.
 */
#define UCI_MAX_PACKET_SIZE (UCI_PACKET_HEADER_SIZE + UCI_MAX_PAYLOAD_SIZE)

/**
 * UCI_STATUS_PACKET_SIZE - Size of a UCI status packet.
 *
 */
#define UCI_STATUS_PACKET_SIZE 5

/**
 * UCI_MT_GID_OID() - Macro to build &uci_message_handler.mt_gid_oid.
 * @mt: Message type.
 * @gid: Group identifier.
 * @oid: Opcode identifier.
 *
 * Return: Integer suitable for use in &uci_message_handler.mt_gid_oid.
 */
#define UCI_MT_GID_OID(mt, gid, oid) ((mt) << 13 | (gid) << 8 | (oid))

/**
 * UCI_GID_OID() - Macro to build @gid_oid parameter.
 * @gid: Group identifier.
 * @oid: Opcode identifier.
 *
 * Return: Integer suitable for use as @gid_oid parameter.
 */
#define UCI_GID_OID(gid, oid) UCI_MT_GID_OID(0, (gid), (oid))

/**
 * UCI_MT() - Macro to extract message type from a @mt_gid_oid parameter.
 * @mt_gid_oid: Union of message type, group identifier and opcode identifier.
 *
 * Return: Message type.
 */
#define UCI_MT(mt_gid_oid) (((mt_gid_oid) >> 13) & 0x7)

/**
 * UCI_GID() - Macro to extract group identifier from a @mt_gid_oid parameter.
 * @mt_gid_oid: Union of message type, group identifier and opcode identifier.
 *
 * Return: Group identifier.
 */
#define UCI_GID(mt_gid_oid) (((mt_gid_oid) >> 8) & 0xf)

/**
 * UCI_OID() - Macro to extract opcode identifier from a @mt_gid_oid parameter.
 * @mt_gid_oid: Union of message type, group identifier and opcode identifier.
 *
 * Return: Opcode identifier.
 */
#define UCI_OID(mt_gid_oid) ((mt_gid_oid)&0x3f)

/**
 * UCI_GID_OID_ONLY() - Extract group and opcode identifier from a @mt_gid_oid
 * parameter.
 * @mt_gid_oid: Union of message type, group identifier and opcode identifier.
 *
 * Return: Group identifier and opcode identifier.
 */
#define UCI_GID_OID_ONLY(mt_gid_oid) ((mt_gid_oid)&0xf3f)

/**
 * UCI_MT_DPF() - Macro to build &uci_message_handler.mt_dpf.
 * @mt: Message type.
 * @dpf: Data Packet Format
 *
 * Return: Integer suitable for use in &uci_message_handler.mt_dpf.
 */
#define UCI_MT_DPF(mt, dpf) ((mt) << 13 | (dpf) << 8)

/**
 * enum uci_blk_flags - Packet buffer block flag.
 *
 * @UCI_BLK_FLAGS_STATIC:
 *     Do not release this block.
 * @UCI_BLK_FLAGS_DESTRUCTIBLE:
 *     Do not release this block with the free method from the given
 *     uci_allocator, but use a custom destructor.
 *     In this case the block should be cast to &struct uci_blk_destructible
 *     to access the destructor.
 * @UCI_BLK_FLAGS_HEADER_RESERVED:
 *     Set if there are 4 reserved bytes for header before payload at the
 *     beginning of the data buffer.
 * @UCI_BLK_FLAGS_REPORT:
 *     Set if the uci block is allocated the UCI report quota.
 */
enum uci_blk_flags {
	UCI_BLK_FLAGS_STATIC = 1,
	UCI_BLK_FLAGS_DESTRUCTIBLE = 2,
	UCI_BLK_FLAGS_HEADER_RESERVED = 4,
	UCI_BLK_FLAGS_REPORT = 8,
};

/**
 * struct uci_blk - Packet buffer block.
 */
struct uci_blk {
	/**
	 * @next: Pointer to next block, or NULL if last one.
	 */
	struct uci_blk *next;
	/**
	 * @data: Pointer to data in this block. This can be changed during the
	 * lifetime of the buffer and should not be used for memory release.
	 */
	uint8_t *data;
	/**
	 * @len: Length of data in this block.
	 */
	uint16_t len;
	/**
	 * @total_len: Length of the full packet or payload. This is set in the
	 * first block only, and zero for other blocks.
	 */
	uint16_t total_len;
	/**
	 * @size: Allocation size for data in this block.
	 */
	uint16_t size;
	/**
	 * @flags: Block flags, see &enum uci_blk_flags.
	 */
	uint8_t flags;
};

/**
 * struct uci_blk_destructible - Packet buffer block with a destructor.
 *
 * This is not allocated using the UCI allocator. It can be used by an external
 * code to embed data inside a packet and avoid copy.
 */
struct uci_blk_destructible {
	/**
	 * @blk: Block common part.
	 */
	struct uci_blk blk;
	/**
	 * @destructor: Function to call instead of &uci_allocator_ops.free.
	 */
	void (*destructor)(void *arg, struct uci_blk_destructible *blk);
	/**
	 * @destructor_arg: First argument of &uci_blk_destructible.destructor.
	 */
	void *destructor_arg;
};

struct uci_allocator;

/**
 * struct uci_allocator_ops - UCI allocator operations.
 */
struct uci_allocator_ops {
	/**
	 * @alloc: Allocate one block. The returned block can be smaller or
	 * larger than the size_hint. Return NULL if memory is exhausted.
	 */
	struct uci_blk *(*alloc)(struct uci_allocator *allocator, size_t size_hint,
				 uint8_t flags_hint);
	/**
	 * @free: Release one block.
	 */
	void (*free)(struct uci_allocator *allocator, struct uci_blk *blk);
};

/**
 * struct uci_allocator - UCI allocator instance.
 */
struct uci_allocator {
	/**
	 * @ops: Allocator operations.
	 */
	const struct uci_allocator_ops *ops;
};

struct uci;

/**
 * struct uci_data_msg_info - Info for data message reception, only applicable
 * to UCI_MESSAGE_TYPE_DATA
 */
struct uci_data_msg_info {
	/**
	 * @is_last_packet: True if currently received data segment is the last
	 * one.
	 */
	bool is_last_packet;
	/**
	 * @is_last_packet_prev: True if previously received data segment was
	 * the last one.
	 */
	bool is_last_packet_prev;
	/**
	 * @app_data_len: Length of data payload in whole Data Message (all
	 * segments).
	 */
	uint16_t app_data_len;
	/**
	 * @app_data_rem_len: Remaining length of data payload to be sent.
	 */
	uint16_t app_data_rem_len;
	/**
	 * @msg_session_handle: Stores sessions ID for whole Data Message.
	 */
	uint32_t msg_session_handle;
};

/**
 * clear_uci_msg_info() - Resets uci_data_msg_info structure in UCI context.
 * @uci: UCI context.
 *
 * Return: None
 */
void clear_uci_msg_info(struct uci *uci);

/**
 * uci_init() - Initialize UCI core.
 * @uci: UCI context.
 * @allocator: Allocator for buffers.
 * @is_client: True if this is a client.
 *
 * Return: QERR_SUCCESS on success, or an error code.
 */
enum qerr uci_init(struct uci *uci, struct uci_allocator *allocator, bool is_client);

/**
 * uci_uninit() - Uninitialize UCI core.
 * @uci: UCI context.
 *
 * Note that uninit does not cleanup the handlers linked list, which should
 * no longer be used outside of the library.
 */
void uci_uninit(struct uci *uci);

/**
 * uci_blk_alloc() - Allocate buffer block.
 * @uci: UCI context.
 * @size_hint: Hint on the required size.
 * @flags_hint: Hint on the flags.
 *
 * Allocate a buffer, usually used to send a message. The size hint can give an
 * indication of the required size. The returned buffer can be smaller or larger
 * due to allocation constrains. In all cases, only one block is returned.
 *
 * The UCI message API should be preferred to build a message to send.
 *
 * Return: The allocated buffer or NULL if no memory is available.
 */
struct uci_blk *uci_blk_alloc(struct uci *uci, size_t size_hint, uint8_t flags_hint);

/**
 * uci_blk_free_all() - Release a chain of buffer blocks.
 * @uci: UCI context.
 * @blks: Chain of buffer blocks, or NULL.
 *
 * Release a chain of buffer blocks which were allocated from uci_blk_alloc() or
 * received by a message handler.
 *
 * Does nothing if called with a NULL pointer.
 */
void uci_blk_free_all(struct uci *uci, struct uci_blk *blks);

/**
 * typedef uci_message_handler_function - Handle a received message.
 * @uci: UCI context.
 * @mt_gid_oid: Union of message type, group identifier and opcode identifier.
 * @payload: Reassembled message payload, can span several buffer blocks.
 * @user_data: User data given at registration.
 *
 * If the handler returns 0, it musts take care of the response if needed,
 * during its execution, or later.
 *
 * If the message is a command and the return value signals an error, a response
 * with status FAILED will be sent.
 *
 * Return: QERR_SUCCESS on success, or an uwbmac error code.
 */
typedef enum qerr (*uci_message_handler_function)(struct uci *uci, uint16_t mt_gid_oid,
						  const struct uci_blk *payload, void *user_data);

/**
 * struct uci_message_handler - Definition of a message handler.
 */
struct uci_message_handler {
	union {
		/**
		 * @uci_message_id: Generic ID of UCI message. Is equal to:
		 * mt_gid_oid - for control packets;
		 * mt_dpf - for data packets;
		 * mt_padded - for SE Testing Packets
		 */
		uint16_t uci_message_id;
		/**
		 * @mt_gid_oid: Union of message type, group identifier and
		 * opcode identifier. Use the UCI_MT_GID_OID() macro.
		 */
		uint16_t mt_gid_oid;
		/**
		 * @mt_dpf: Union of message type and Data Packet Format. Use
		 * the UCI_MT_DPF() macro.
		 */
		uint16_t mt_dpf;
		/**
		 * @mt_padded: Padded with zeros message type only. Use
		 * UCI_MT_GID_OID() with gid=0 and oid=0 or UCI_MT_DPF() with
		 * dpf=0.
		 */
		uint16_t mt_padded;
	};
	/**
	 * @handler: Function called to handle a received message.
	 */
	uci_message_handler_function handler;
};

/**
 * struct uci_message_handlers - Definition of several message handlers
 * belonging to the same module.
 */
struct uci_message_handlers {
	/**
	 * @next: Pointer to next message handlers definition, or NULL if this
	 * is the last one. Filled at registration.
	 */
	struct uci_message_handlers *next;
	/**
	 * @handlers: Pointer to array of message handler definitions.
	 *
	 * NOTE: **This must be sorted** by message type, then group identifier,
	 * then opcode identifier.
	 */
	const struct uci_message_handler *handlers;
	/**
	 * @n_handlers: Number of message handlers.
	 */
	size_t n_handlers;
	/**
	 * @user_data: User data given to message handlers when a message is
	 * received.
	 */
	void *user_data;
};

// TODO(cyril): in unit test handlers memory is discarded before uci_uninit()
// TODO(cyril): implement an unregister() to losen the memory requirement
/**
 * uci_message_handlers_register() - Register message handlers.
 * @uci: UCI context.
 * @handlers: Information on message handlers.
 *
 * Be sure that the uci_message_handlers provided only contains handlers for
 * only one GID.
 * The method will not accept it otherwise and will register nothing.
 *
 * NOTE: Handlers memory is managed by the client, but its representation
 * is managed as a linked list by the library. Its memory is assumed to be
 * usable until uci_uninit().
 *
 */
void uci_message_handlers_register(struct uci *uci, struct uci_message_handlers *handlers);

/**
 * uci_message_handlers_unregister() - Unregister message handlers.
 * @uci: UCI context.
 * @handlers: Information on message handlers.
 *
 * Remove the specified uci_message_handlers from the linked list.
 */
void uci_message_handlers_unregister(struct uci *uci, struct uci_message_handlers *handlers);

/**
 * uci_packet_recv_alloc() - Allocate buffer for reception.
 * @uci: UCI context.
 * @size_hint: Hint on the required size.
 *
 * This function should be called by the transport channel to build packets for
 * received data. The size hint can give an indication of the required size in
 * case the packet size is known. The returned buffer can be smaller or larger
 * due to allocation constrains. In all cases, only one block is returned.
 *
 * Return: The allocated buffer or NULL if no memory is available.
 */
struct uci_blk *uci_packet_recv_alloc(struct uci *uci, uint16_t size_hint);

/**
 * uci_packet_recv_free_all() - Release unused reception buffer.
 * @uci: UCI context.
 * @blks: Chain of buffer blocks to release, or NULL.
 *
 * This function should be called by the transport channel with unused buffers
 * when a packet reception was aborted due to an error, or when the channel is
 * shut down.
 *
 * Does nothing if called with a NULL pointer.
 */
void uci_packet_recv_free_all(struct uci *uci, struct uci_blk *blks);

/**
 * uci_packet_recv() - Hand off valid received packet.
 * @uci: UCI context.
 * @packet: Received packet (can be composed of several blocks).
 *
 * This function should be called by the transport channel when a packet has
 * been successfully received. The UCI core takes ownership of the associated
 * memory.
 *
 */
void uci_packet_recv(struct uci *uci, struct uci_blk *packet);

struct uci_transport;

/**
 * struct uci_transport_ops - UCI transport channel callbacks.
 */
struct uci_transport_ops {
	/**
	 * @attach: Callback invoked when the transport channel is attached.
	 */
	void (*attach)(struct uci_transport *uci_tr, struct uci *uci);
	/**
	 * @detach: Callback invoked when the transport channel is detached. All
	 * lent buffers should be released (using uci_packet_recv_free() and
	 * uci_packet_send_done()).
	 */
	void (*detach)(struct uci_transport *uci_tr);
	/**
	 * @packet_send_ready: Callback invoked when UCI wants to send a packet
	 * initially or after &uci_packet_send_get_ready() returned NULL. It
	 * allows to restart the sending data pump.
	 */
	void (*packet_send_ready)(struct uci_transport *uci_tr);
};

/**
 * struct uci_transport - UCI generic transport channel.
 */
struct uci_transport {
	/**
	 * @ops: Transport channel callbacks.
	 */
	const struct uci_transport_ops *ops;
};

/**
 * uci_transport_attach() - Attach a transport channel.
 * @uci: UCI context.
 * @uci_tr: Transport channel to attach.
 *
 * Return: QERR_SUCCESS on success, or QERR_EBUSY if a transport is already
 * attached.
 */
enum qerr uci_transport_attach(struct uci *uci, struct uci_transport *uci_tr);

/**
 * uci_transport_detach() - Detach a transport channel.
 * @uci: UCI context.
 *
 * Return: QERR_SUCCESS on success, or QERR_EINVAL if no transport is attached.
 */
enum qerr uci_transport_detach(struct uci *uci);

/**
 * uci_send_message() - Send a message.
 * @uci: UCI context.
 * @uci_message_id: a) For Control message it's union of message type, group
 * identifier and opcode identifier. Use the UCI_MT_GID_OID() macro. b) For Data
 * message it's union of message type and Data Packet Format. Use the
 * UCI_MT_DPF() macro.
 * @payload: Message payload which can be composed of several buffer blocks, or
 * NULL if there is none. Ownership is transferred to UCI core.
 *
 * The message payload is segmented, packets are built and queued to be sent. If
 * you use the UCI message API, this is done efficiently as room is reserved
 * during message construction for headers.
 */
void uci_send_message(struct uci *uci, uint16_t uci_message_id, struct uci_blk *payload);

/**
 * uci_send_se_message() - Send a message for the SE.
 * @uci: UCI context.
 * @mt: The MT of the SE message.
 * @payload: Message payload or NULL if there is none.
 *           Ownership is transferred to UCI core.
 */
void uci_send_se_message(struct uci *uci, enum uci_message_type mt, struct uci_blk *payload);

/**
 * uci_send_status() - Send a status response.
 * @uci: UCI context.
 * @gid_oid: Union of group identifier and opcode identifier. Use the
 *           UCI_GID_OID() macro, message type is ignored.
 * @status: Status code.
 */
void uci_send_status(struct uci *uci, uint16_t gid_oid, uint8_t status);

/**
 * uci_get_device_state() - Get the current device state
 * @uci: UCI context.
 *
 * Note: This is only available for server mode.
 * Return: The current device state.
 */
uint8_t uci_get_device_state(struct uci *uci);

/**
 * uci_set_device_state_notification() - Set the device state and send
 * notifiction if needed
 * @uci: UCI context.
 * @state: The state to set.
 *
 * This is only available for server mode.
 */
void uci_set_device_state_notification(struct uci *uci, uint8_t state);

/**
 * uci_reset_device_state() - Reset the device state to ready.
 * @uci: UCI context.
 *
 * This should be used once the server is fully setup after each server reset.
 * Note: This is only available for server mode.
 */
void uci_reset_device_state(struct uci *uci);

/**
 * uci_send_device_boot_notification() - Send device boot notification.
 * @uci: UCI context.
 * @boot_reason: Reason of the boot.
 *
 * This should be used once the server is fully boot.
 * Note: This is only available for server mode.
 */
void uci_send_device_boot_notification(struct uci *uci, enum uci_qorvo_boot_reason boot_reason);

/**
 * uci_packet_send_get_ready() - Retrieve the packet ready to be sent.
 * @uci: UCI context.
 *
 * Packet buffers are lent to the transport channel, they should be returned
 * once transmission is done using uci_packet_send_done().
 *
 * If there is no packet to send, NULL is returned and UCI core will signal any
 * new pending packet using the &uci_transport_ops.packet_send_ready callback.
 *
 * Return: The first block of the packet to send, or NULL if no packet to send.
 */
struct uci_blk *uci_packet_send_get_ready(struct uci *uci);

/**
 * uci_packet_send_done() - Signal the packet has been transmitted.
 * @uci: UCI context.
 * @packet: Packet that was sent, or that failed to be sent.
 * @status: 0 if transmission was successful, or a negative error code.
 *
 * Once a packet has been transmitted, or after a non recoverable failure,
 * packet should be returned by the transport channel using this function (see
 * &uci_packet_send_get_ready and &uci_transport_ops.packet_send_ready).
 */
void uci_packet_send_done(struct uci *uci, struct uci_blk *packet, int status);

// TODO(Guillaume): not implemented
/**
 * uci_packet_response_expire() - Signal the response timer expired.
 * @uci: UCI context.
 *
 * This is only needed for UCI client.
 *
 * The transport channel should reset a timer each time a packet is sent on the
 * transport channel. If the timer expires, it should call this function. This
 * is used to identify problem with a missing response.
 *
 * The timer expiration will be ignored if UCI core has a pending packet which
 * was not given back using uci_packet_send_done().
 *
 * The timer duration is specific to the transport channel.
 */
void uci_packet_response_expire(struct uci *uci);

/**
 * struct uci - UCI core context.
 */
struct uci {
	/**
	 * @allocator: Allocator to use to allocate buffer blocks.
	 */
	struct uci_allocator *allocator;
	/**
	 * @handlers_head: Head of the list of handlers.
	 */
	struct uci_message_handlers *handlers_head;
	/**
	 * @rx: Used to collect segmented messages before processing.
	 */
	struct uci_blk *rx;
	/**
	 * @rx_last: Last received segmented message before processing.
	 */
	struct uci_blk *rx_last;
	/**
	 * @tx: Pointer to first message to send of TX queue.
	 */
	struct uci_blk *tx;
	/**
	 * @tx_last: Pointer to last message to send of TX queue.
	 */
	struct uci_blk *tx_last;
	/**
	 * @tr: Transport channel attached to this context or NULL.
	 */
	struct uci_transport *tr;
	/**
	 * @rsp_blk: Preallocated block for RSP message or NULL if no memory was
	 * 			 available.
	 */
	struct uci_blk *rsp_blk;
	/**
	 * @is_client: True if this is a client.
	 */
	bool is_client;
	/**
	 * @known_gid: Bitfield of known GID in message handlers list.
	 */
	uint16_t known_gid;
	/**
	 * @device_state: the UCI device state.
	 */
	enum uci_device_state device_state;
	/**
	 * @data_msg_info: Keeps information about current data message
	 * (UCI_MESSAGE_TYPE_DATA) being received.
	 */
	struct uci_data_msg_info data_msg_info;
	/**
	 * @mutex: UCI mutex.
	 */
	struct qmutex *mutex;
};

/**
 * uci_packet_hdr_get_payload_size() - Get the payload size from the UCI header
 * @hdr: The header from of the UCI message, needs to be at least of size
 * 		UCI_PACKET_HEADER_SIZE.
 *
 * Return: the size of the payload.
 */
static inline uint16_t uci_packet_hdr_get_payload_size(const uint8_t *hdr)
{
	enum uci_message_type mt = (enum uci_message_type)(hdr[0] >> 5);

	switch (mt) {
	case UCI_MESSAGE_TYPE_DATA:
	case UCI_MESSAGE_TYPE_SE_TESTING_COMMAND:
	case UCI_MESSAGE_TYPE_SE_TESTING_RESPONSE:
		return (hdr[3] << 8) | hdr[2];
	case UCI_MESSAGE_TYPE_COMMAND:
	case UCI_MESSAGE_TYPE_RESPONSE:
	case UCI_MESSAGE_TYPE_NOTIFICATION:
	default:
		return hdr[3];
	}
}
