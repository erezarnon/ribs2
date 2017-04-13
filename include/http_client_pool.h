/*
    This file is part of RIBS2.0 (Robust Infrastructure for Backend Systems).
    RIBS is an infrastructure for building great SaaS applications (but not
    limited to).

    Copyright (C) 2012,2013,2014 Adap.tv, Inc.

    RIBS is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, version 2.1 of the License.

    RIBS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with RIBS.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _HTTP_CLIENT_POOL__H_
#define _HTTP_CLIENT_POOL__H_

#include "ribs_defs.h"
#include "ctx_pool.h"
#include "vmbuf.h"
#include "vmfile.h"
#include "timeout_handler.h"
#include "logger.h"
#include <netinet/in.h>

struct http_client_pool {
    struct ctx_pool ctx_pool;
    struct timeout_handler timeout_handler;
    struct timeout_handler timeout_handler_persistent;
#ifdef RIBS2_SSL
    SSL_CTX *ssl_ctx;
    int check_cert;
#endif
};

struct http_client_context {
    int fd;
    uint32_t content_length;
    struct vmbuf request;
    struct vmbuf response;
    char *content;
    struct http_client_pool *pool;
    epoll_data_t userdata;
    short http_status_code;
#ifdef RIBS2_SSL
    const char *hostname;
    char ssl_connected;
#endif
    char persistent;
    /* TODO: add initial buffer sizes */

    /* keep 1 byte aligned structs last */
#pragma pack(push, 1)
    struct http_client_key {
        struct in_addr addr;
        uint16_t port;
        uint32_t hostname_hash;
    } key;
#pragma pack(pop)
};

int http_client_pool_init(struct http_client_pool *http_client_pool, size_t initial, size_t grow);
#ifdef RIBS2_SSL
int http_client_pool_init_ssl(struct http_client_pool *http_client_pool, size_t initial, size_t grow, char *cacert);
#endif
void http_client_free(struct http_client_context *cctx);
struct http_client_context *http_client_pool_create_client(struct http_client_pool *http_client_pool, struct in_addr addr, uint16_t port, struct ribs_context *rctx);
struct http_client_context *http_client_pool_create_client2(struct http_client_pool *http_client_pool, struct in_addr addr, uint16_t port, const char *hostname, struct ribs_context *rctx);
int http_client_pool_get_request(struct http_client_pool *http_client_pool, struct in_addr addr, uint16_t port, const char *hostname, const char *format, ...) __attribute__ ((format (gnu_printf, 5, 6)));
int http_client_pool_get_request2(struct http_client_pool *http_client_pool, struct in_addr addr, uint16_t port, const char *hostname, const char **headers, const char *format, ...) __attribute__ ((format (gnu_printf, 6, 7)));
int http_client_pool_post_request(struct http_client_pool *http_client_pool, struct in_addr addr, uint16_t port, const char *hostname, const char *data, size_t size_of_data, const char *format, ...) __attribute__ ((format (gnu_printf, 7, 8)));
struct http_client_context *http_client_pool_get_request3(struct http_client_pool *http_client_pool, struct in_addr addr, uint16_t port, const char *hostname, const char **headers, const char *format, ...);
struct http_client_context *http_client_pool_post_request_init(struct http_client_pool *http_client_pool, struct in_addr addr, uint16_t port, const char *hostname, const char *format, ...) __attribute__ ((format (gnu_printf, 5, 6)));
int http_client_pool_post_request_send(struct http_client_context *context, struct vmbuf *post_data);
int http_client_get_file(struct http_client_pool *http_client_pool, struct vmfile *infile, struct in_addr addr, uint16_t port, const char *hostname, int compression, int * file_compressed, const char *format, ...) __attribute__ ((format (gnu_printf, 8, 9)));
struct http_client_context *http_client_get_last_context(void);
_RIBS_INLINE_ struct ribs_context *http_client_get_ribs_context(struct http_client_context *cctx);
_RIBS_INLINE_ int http_client_send_request(struct http_client_context *cctx);
_RIBS_INLINE_ char *http_client_response_headers(struct http_client_context *cctx);
void http_client_reuse_context(struct http_client_context *ctx);

#include "../src/_http_client_pool.c"

#endif // _HTTP_CLIENT_POOL__H_
