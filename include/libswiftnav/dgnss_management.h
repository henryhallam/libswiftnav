/*
 * Copyright (C) 2014 Swift Navigation Inc.
 * Contact: Ian Horn <ian@swift-nav.com>
 *
 * This source is subject to the license found in the file 'LICENSE' which must
 * be be distributed together with this source. All other rights reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef LIBSWIFTNAV_DGNSS_MANAGEMENT_H
#define LIBSWIFTNAV_DGNSS_MANAGEMENT_H

#include "amb_kf.h"
#include "sats_management.h"
#include "ambiguity_test.h"
#include "baseline.h"

/* The default DD carrier phase variance to use in the hypothesis testing. */
#define DEFAULT_PHASE_VAR_TEST  (9e-4 * 16)
/* The default DD pseudorange variance to use in the hypothesis testing. */
#define DEFAULT_CODE_VAR_TEST   (100 * 400)
/* The default DD carrier phase variance to use in the Kalman filter. */
#define DEFAULT_PHASE_VAR_KF    (9e-4 * 16)
/* The default DD pseudorange variance to use in the Kalman filter. */
#define DEFAULT_CODE_VAR_KF     (100 * 400)
/* The default variance of the process noise Kalman filter. Its particular use
 * is different from that of a normal KF process noise. It's still a random
 * walk, but in a special space. Look at the code for its current usage.*/
#define DEFAULT_AMB_DRIFT_VAR   1e-8
/* The variance with which to initialize the Kalman Filter. */
#define DEFAULT_AMB_INIT_VAR    1e25
/* The variance with which to add new sats to the Kalman Filter.
 * TODO deprecate in lieu of amb_init_var once we do some tuning. */
#define DEFAULT_NEW_INT_VAR     1e25

typedef struct {
  double phase_var_test;
  double code_var_test;
  double phase_var_kf;
  double code_var_kf;
  double pos_trans_var;
  double vel_trans_var;
  double int_trans_var;
  double amb_drift_var;
  double pos_init_var;
  double vel_init_var;
  double amb_init_var;
  double new_int_var;
} dgnss_settings_t;

typedef struct {
  ambiguities_t fixed_ambs;
  ambiguities_t float_ambs;
} ambiguity_state_t;

extern dgnss_settings_t dgnss_settings;

void dgnss_set_settings(double phase_var_test, double code_var_test,
                        double phase_var_kf, double code_var_kf,
                        double amb_drift_var, double amb_init_var,
                        double new_int_var);
void make_measurements(u8 num_diffs, const sdiff_t *sdiffs, double *raw_measurements);
void dgnss_init(u8 num_sats, sdiff_t *sdiffs, double reciever_ecef[3]);
void dgnss_update(u8 num_sats, sdiff_t *sdiffs, double reciever_ecef[3]);
void dgnss_rebase_ref(u8 num_sats, sdiff_t *sdiffs, double reciever_ecef[3],
                      u8 old_prns[MAX_CHANNELS], sdiff_t *corrected_sdiffs);
nkf_t * get_dgnss_nkf(void);
sats_management_t * get_sats_management(void);
ambiguity_test_t* get_ambiguity_test(void);

s8 dgnss_iar_resolved(void);
u32 dgnss_iar_num_hyps(void);
u32 dgnss_iar_num_sats(void);
s8 dgnss_iar_get_single_hyp(double *hyp);
void dgnss_reset_iar(void);
void dgnss_init_known_baseline(u8 num_sats, sdiff_t *sdiffs, double receiver_ecef[3], double b[3]);
void dgnss_update_ambiguity_state(ambiguity_state_t *s);
s8 dgnss_baseline(u8 num_sdiffs, const sdiff_t *sdiffs,
                  const double ref_ecef[3], const ambiguity_state_t *s,
                  u8 *num_used, double b[3]);
void measure_amb_kf_b(u8 num_sdiffs, sdiff_t *sdiffs,
                      const double receiver_ecef[3], double *b);
void measure_b_with_external_ambs(u8 state_dim, const double *state_mean,
                                  u8 num_sdiffs, sdiff_t *sdiffs,
                                  const double receiver_ecef[3], double *b);
void measure_iar_b_with_external_ambs(double *state_mean,
                                      u8 num_sdiffs, sdiff_t *sdiffs,
                                      double receiver_ecef[3],
                                      double *b);
u8 get_amb_kf_de_and_phase(u8 num_sdiffs, sdiff_t *sdiffs,
                           double ref_ecef[3],
                           double *de, double *phase);
u8 get_iar_de_and_phase(u8 num_sdiffs, sdiff_t *sdiffs,
                        double ref_ecef[3],
                        double *de, double *phase);
u8 dgnss_iar_pool_contains(double *ambs);
double dgnss_iar_pool_ll(u8 num_ambs, double *ambs);
double dgnss_iar_pool_prob(u8 num_ambs, double *ambs);
u8 get_amb_kf_mean(double *ambs);
u8 get_amb_kf_cov(double *cov);
u8 get_amb_kf_prns(u8 *prns);
u8 get_amb_test_prns(u8 *prns);
u8 dgnss_iar_MLE_ambs(s32 *ambs);

#endif /* LIBSWIFTNAV_DGNSS_MANAGEMENT_H */
