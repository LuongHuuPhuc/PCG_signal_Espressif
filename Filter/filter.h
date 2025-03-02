/**
 * @brief Thu vien bo loc nhieu dien luoi 50Hz IIR Notch Biquad 2 tang va Low Pass Butterworth
 * @author Luong Huu Phuc - 20224452
 * @date 2025/02/22
 */
#ifndef FILTER_H_
#define FILTER_H_

#include <stdio.h>
#include <stdint.h>

#define NOTCH_FREQ1 50.0f //50Hz
#define NOTCH_FREQ2 100.0f //Boi so cua 50Hz
#define BTTW_CUTOFF 500.0f //Tan so cat cua Butterworth (co the thay doi)
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

/**--------------------------Cau truc bo loc Butterworth----------------------------- */
typedef struct {
  float a_0, a_1, a_2; //Cac he so Biquad
  float b_0, b_1, b_2; //Cac he so Biquad
  float x_1, x_2; //Luu tru trang thai dau vao
  float y_1, y_2; //Luu tru trang thai dau ra
} butterworth_filter_t;

//Khai bao 2 bo loc Notch (Loc 2 tang cascade)
extern notch_filter_t notch_filter, notch_filter1, notch_filter2;
extern butterworth_filter_t butterworth_filter;

/**
 * @brief Ham khoi tao bo loc - tinh toan he so Notch filter
 * @param filter Khoi tao cac tham so cua bo loc 
 * @param notch_sample_rate Tan so lay mau du lieu 
 * @param notch_freq Tan so trung tam - can loai bo (50Hz...)
 * @param q_pram He so chat luong, q cang cao bang thong cang hep, bi cat cang ro
 */
void notch_filter_init(notch_filter_t *filter, float notch_sample_rate, float notch_freq, float q_param);

/**
 * @brief Ham ap dung bo loc Notch len du lieu dau vao cho moi mau am thanh
 * @param sample[out] So mau xuat ra 
 * @param filter1 Bo loc thu nhat 
 * @param filter2 Bo loc thu hai 
 */
float notch_filter_apply(notch_filter_t *filter1, notch_filter_t *filter2, float input_sample);

/**
 * @brief Ham khoi tao bo loc Butterworth de loai bo tan so cao tren 500Hz
 * @note Lam muot tin hieu 
 * @param filter Bo loc butterworth
 * @param sample_rate Tan so lay mau
 * @param cutoff_freq Tan so cat (Loai bo tan so tu tan so cat tro len)
 */
void butterworth_filter_init(butterworth_filter_t *filter, float sample_rate, float cutoff_freq);

/**
 * @brief Ham ap dung bo loc Butterworth
 * @param input_sample Data sau khi duoc xu ly tu Notch
 * @param filter Bo loc Butterworth
 */
float butterworth_filter_apply(butterworth_filter_t *filter, float input_sample);

/** 
 * @note Ham cuoi cung: ap dung tat ca cac bo loc va tra ve 16-bit
 * @return Tra ve gia tri 16-bit
 */
int16_t filter_process(int16_t input_sample);

#endif //FILTER_H_