/**
 * @brief Thu vien bo loc nhieu dien luoi 50Hz IIR Notch Biquad 2 tang va BandPass Biquad IIR filter 
 * @author Luong Huu Phuc - 20224452
 * @date 2025/02/22
 */
#ifndef FILTER_H_
#define FILTER_H_

#pragma once

#include <stdio.h>
#include <stdint.h>

#define NOTCH_FREQ1 50.0f //50Hz
#define NOTCH_FREQ2 100.0f //Boi so cua 50Hz
#define Q_PARAM 50.0f
#define PI 3.14159265358979323846

/**----------------------------Cau truc bo loc Notch------------------------------- */
typedef struct {
  float notch_sample_rate; //Tan so lay mau
  float notch_freq; //Tan so chan 
  float q_param; //He so chat luong cua bo loc Notch
  float a0, a1, a2; //Cac he so Biquad
  float b0, b1, b2; //Cac he so Biquad
  float x1, x2; //Luu tru trang thai dau vao
  float y1, y2; //Luu tru trang thai dau ra
} notch_filter_t;

/**--------------------------Cau truc bo loc Low-Pass Butterworth----------------------------- */
typedef struct {
  float a_0, a_1, a_2; //Cac he so Biquad
  float b_0, b_1, b_2; //Cac he so Biquad
  float x_1, x_2; //Luu tru trang thai dau vao
  float y_1, y_2; //Luu tru trang thai dau ra
} btw_lowPass_filter_t;

/**--------------------------Cau truc bo loc High-Pass Butterworth---------------------------- */
typedef struct {
  float a_0_, a_1_, a_2_; //Cac he so Biquad cho HighPass
  float b_0_, b_1_, b_2_;
  float x_1_, x_2_;
  float y_1_, y_2_;
} btw_highPass_filter_t;

//Khai bao 2 bo loc Notch (Loc 2 tang cascade)
extern notch_filter_t notch_filter, notch_filter1, notch_filter2;
extern btw_lowPass_filter_t butterworth_low_pass_filter, 
                            butterworth_low_pass_filter1, butterworth_low_pass_filter2, 
                            butterworth_low_pass_filter3, butterworth_low_pass_filter4;
extern btw_highPass_filter_t butterworth_high_pass_filter, 
                             butterworth_high_pass_filter1, butterworth_high_pass_filter2, 
                             butterworth_high_pass_filter3, butterworth_high_pass_filter4;

/**
 * @brief Ham khoi tao bo loc - tinh toan he so Notch filter
 * @param filter Khoi tao cac tham so cua bo loc 
 * @param notch_sample_rate Tan so lay mau du lieu 
 * @param notch_freq Tan so trung tam - can loai bo (50Hz...)
 * @param q_pram He so chat luong, q cang cao bang thong cang hep, bi cat cang ro
 */
extern void notch_filter_init(notch_filter_t *filter, float notch_sample_rate, float notch_freq, float q_param);

/**
 * @brief Ham ap dung bo loc Notch len du lieu dau vao cho moi mau am thanh
 * @param sample[out] So mau xuat ra 
 * @param filter1 Bo loc thu nhat 
 * @param filter2 Bo loc thu hai 
 */
extern float notch_filter_apply(notch_filter_t *filter1, notch_filter_t *filter2, float input_sample);

/**
 * @brief Ham khoi tao bo loc low pass de loai bo nhieu cao tan (bac 1)
 * @note Lam muot tin hieu 
 * @param filter Bo loc butterworth
 * @param sample_rate Tan so lay mau
 * @param cutoff_freq Tan so cat (Loai bo tan so tu tan so cat tro len)
 */
extern void btw_lowPass_filter_init(btw_lowPass_filter_t *filter, float sample_rate, float cutoff_freq);

/**
 * @brief Ham ap dung bo loc low pass
 * @param input_sample Data sau khi duoc xu ly tu Notch
 * @param filter Bo loc Butterworth
 */
extern float btw_lowPass_filter_apply(btw_lowPass_filter_t *filter, float input_sample);

/**
 * @brief Ham ap dung boo loc low pass (bac 4)
 */
int16_t btw_lowPass_filter_biquad_cascade_4th(btw_lowPass_filter_t *filter1, btw_lowPass_filter_t *filter2, 
  btw_lowPass_filter_t *filter3, btw_lowPass_filter_t *filter4, int16_t input_sample);

/**
 * @brief Ham khoi tao bo loc high pass de loc thong cao (bac 1)
 * @note Lam muot tin hieu 
 * @param filter Bo loc butterworth
 * @param sample_rate Tan so lay mau
 * @param cutoff_freq Tan so cat (Loai bo tan so tu tan so cat tro xuong)
 */
extern void btw_highPass_filter_init(btw_highPass_filter_t *filter, float sample_rate, float cutoff_freq);

/**
 * @brief Ham ap dung bo loc highPass
 * @param input_sample Data sau khi duoc xu ly
 * @param filter Bo loc Butterworth
 */
extern float btw_highPass_filter_apply(btw_highPass_filter_t *filter, float input_sample);

/**
 * @brief Ham ap dung bo loc high pass (bac 4)
 */
int16_t btw_highPass_filter_biquad_cascade_4th(btw_highPass_filter_t *filter1, btw_highPass_filter_t *filter2, 
  btw_highPass_filter_t *filter3, btw_highPass_filter_t *filter4, int16_t input_sample);

/** 
 * @note Ham cuoi cung: notch + lowpass
 * @return Tra ve gia tri 16-bit
 */
extern int16_t filter_process(int16_t input_sample);

/** 
 * @note Ham cuoi cung: highpass + lowpass = bandpass (bac 4)
 * @return Tra ve gia tri 16-bit
 */
extern int16_t bandpass_cascade_4th_process(int16_t input_sample);

#endif //FILTER_H_