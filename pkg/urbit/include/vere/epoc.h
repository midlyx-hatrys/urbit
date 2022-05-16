//! @file epoc.h
//!
//! Event log epoch.

#ifndef U3_VERE_EPOC_H
#define U3_VERE_EPOC_H

#include <lmdb.h>

#include "c/portable.h"
#include "c/types.h"
#include "c/list.h"
#include "c/path.h"
#include "vere/meta.h"

//==============================================================================
// Types
//==============================================================================

//! Iterator over an epoch's events.
struct _u3_epoc_iter;
typedef struct _u3_epoc_iter u3_epoc_iter;

//! Epoch.
struct _u3_epoc;
typedef struct _u3_epoc u3_epoc;

//==============================================================================
// Constants
//==============================================================================

//! Current epoch version number.
extern const c3_w epo_ver_w;

//! Size of the `u3_epoc` type.
extern const size_t epo_siz_i;

//! Prefix of epoch directory name.
extern const c3_c epo_pre_c[];

//! Smallest event ID that an epoch will accept.
extern const c3_d epo_min_d;

//==============================================================================
// Macros
//==============================================================================

//! Error handling wrapper for u3_epoc API calls that return 0/NULL on failure
//! and non-zero/non-NULL on success.
//!
//! @param[in] epoc_call       u3_epoc API call.
//! @param[in] failure_action  Statement to execute after logging failure.
#define try_epoc(epoc_call, failure_action, ...)                               \
  do {                                                                         \
    if ( !(epoc_call) ) {                                                      \
      fprintf(stderr, "epoc: " __VA_ARGS__);                                   \
      fprintf(stderr, "\r\n");                                                 \
      failure_action;                                                          \
    }                                                                          \
  } while ( 0 )

//==============================================================================
// Functions
//==============================================================================

//! Create a new empty epoch rooted at `<par_c>/0i<fir_d>`.
//!
//! @param[in] par_u  Parent directory to house epoch. Will be created if it
//!                   doesn't already exist.
//! @param[in] fir_d  Event ID of first event in epoch.
//! @param[in] lif_w  Lifecycle length of boot sequence. Only relevant for first
//!                   epoch (i.e. ignored if `fir_d != epo_min_d`).
//!
//! @return NULL  New epoch could not be created.
//! @return       Handle to new epoch.
u3_epoc*
u3_epoc_new(const c3_path* const par_u, const c3_d fir_d, c3_w lif_w);

//! Create a new first epoch from an existing (non-epoch-based) event log.
//!
//! @note This is intended for use in migration from the non-epoch-based event
//!       log to the new epoch-based event log.
//!
//! @param[in]  par_u  Parent directory to house epoch. Will be created if it
//!                    doesn't already exist.
//! @param[in]  src_u  Path to directory housing existing event log.
//! @param[out] met_u  Pointer to pier metadata.
//!
//! @return NULL  New epoch could not be created from old event log.
//! @return       Handle to migrated epoch.
u3_epoc*
u3_epoc_migrate(const c3_path* const par_u,
                c3_path* const       src_u,
                u3_meta* const       met_u);

//! Load an existing epoch created with u3_epoc_new().
//!
//! @param[in]  pax_u  Root directory of epoch.
//! @param[out] lif_w  Pointer to length of boot sequence. Only relevant for
//!                    first epoch. Can be NULL if not first epoch. If not NULL
//!                    and not first epoch, will be set to 0.
//!
//! @return NULL  Existing epoch could not be opened.
//! @return       Handle to open epoch.
u3_epoc*
u3_epoc_open(const c3_path* const pax_u, c3_w* const lif_w);

//! Get the file path of an epoch.
//!
//! @param[in] poc_u  Epoch handle. Must not be NULL.
//!
//! @return  File path.
const c3_path*
u3_epoc_path(const u3_epoc* const poc_u);

//! Get the string representation of an epoch's file path.
//!
//! @param[in] poc_u  Epoch handle. Must not be NULL.
//!
//! @return  String representation of file path.
const c3_c*
u3_epoc_path_str(const u3_epoc* const poc_u);

//! Get the ID of the first committed event in an epoch.
//!
//! @param[in] poc_u  Epoch handle. Must have at least one committed event and
//!                   must not be NULL.
//!
//! @return  ID of epoch's first committed event.
c3_d
u3_epoc_first_commit(const u3_epoc* const poc_u);

//! Get the ID of the last committed event in an epoch.
//!
//! @param[in] poc_u  Epoch handle. Must have at least one committed event and
//!                   must not be NULL.
//!
//! @return  ID of epoch's last committed event.
c3_d
u3_epoc_last_commit(const u3_epoc* const poc_u);

//! Get the ID of the first event represented by the epoch, which is equivalent
//! to the ID of the event that corresponds to the epoch's snapshot.
//!
//! @param[in] poc_u  Epoch handle. Must not be NULL.
//!
//! @return  ID of epoch's first event.
c3_d
u3_epoc_first_evt(const u3_epoc* const poc_u);

//! Determine whether an epoch has been committed.
//!
//! @param[in] poc_u  Epoch handle. Must not be NULL.
//!
//! @return 0  No events have been committed to `poc_u`.
//! @return 1  Events have been committed to `poc_u`.
c3_t
u3_epoc_is_empty(const u3_epoc* const poc_u);

//! Get the number of committed events in an epoch.
//!
//! @param[in] poc_u  Epoch handle. Must not be NULL.
//!
//! @return  Number of committed events.
size_t
u3_epoc_len(const u3_epoc* const poc_u);

//! Determine if an event ID is contained within an epoch.
//!
//! @param[in] poc_u  Epoch handle. Must not be NULL.
//! @param[in] ide_d  Event ID.
//!
//! @return 0  `ide_d` is not in `poc_u`.
//! @return 1  `ide_d` is in `poc_u`.
c3_t
u3_epoc_has(const u3_epoc* const poc_u, const c3_d ide_d);

//! Determine if an epoch contains the smallest possible event ID (i.e. is the
//! first epoch).
//!
//! @param[in] poc_u  Epoch handle. Must not be NULL.
//!
//! @return 0  `poc_u` is not the first epoch.
//! @return 1  `poc_u` is the first epoch.
c3_t
u3_epoc_is_first(const u3_epoc* const poc_u);

//! Commit one or more serialized events to an epoch.
//!
//! @param[in] poc_u  Epoch handle.
//! @param[in] nod_u  c3_list node of first event to commit with serialized
//!                   event as payload.
//! @param[in] len_i  Number of events to commit.
c3_t
u3_epoc_commit(u3_epoc* const poc_u, const c3_lode* nod_u, const size_t len_i);

//! Open an iterator within an epoch.
//!
//! @param[in] poc_u  Epoch handle.
//! @param[in] ide_d  Event ID to initilize iterator at.
//!
//! @return 0  `poc_u` already has an open iterator.
//! @return 0  `poc_u` doesn't contain event ID `ide_d`.
c3_t
u3_epoc_iter_open(u3_epoc* const poc_u, c3_d ide_d);

//! Get the event at the current iterator and advance the iterator to the next
//! event.
//!
//! @param[in]  poc_u  Must not be NULL.
//! @param[out] byt_y  Pointer to hold serialized event buffer. Must not be
//!                    NULL. `*byt_y` must NOT be freed.
//! @param[out] len_i  Pointer to hold `byt_y` in bytes. Must not be NULL.
//!
//! @return 0  `poc_u` doesn't have an open iterator.
//! @return 0  Already consumed last event in `poc_u`.
c3_t
u3_epoc_iter_step(u3_epoc* const poc_u,
                  c3_y** const   byt_y,
                  size_t* const  len_i);

//! Close an epoch's iterator.
//!
//! @param[in] poc_u  Epoch handle.
void
u3_epoc_iter_close(u3_epoc* const poc_u);

//! Print info about an epoch's LMDB instance.
//!
//! @param[in] poc_u  Epoch handle.
void
u3_epoc_info(const u3_epoc* const poc_u);

//! Gracefully dispose of an epoch's resources. Does not free the epoch handle
//! itself.
//!
//! @param[in] poc_u  Epoch handle.
void
u3_epoc_close(u3_epoc* const poc_u);

#endif /* ifndef U3_VERE_EPOC_H */