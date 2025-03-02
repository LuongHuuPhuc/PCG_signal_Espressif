/**
 * @brief Thu vien bo loc nhieu dien luoi 50Hz IIR Notch Biquad 2 tang va Low Pass Butterworth 
 * @author Luong Huu Phuc - 20224452
 * @date 2025/02/22
 */

#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<math.h>  
#include"filter.h" 

void notch_filter_init(notch_filter_t *filter, float notch_sample_rate, float notch_freq, float q_param){
  filter->notch_sample_rate = notch_sample_rate;
  filter->notch_freq = notch_freq;
  filter->q_param = q_param;

  /**
   * @note Data sau khi tu mic thu vao se duoc chuan hoa theo tan so lay mau 
   * -> Chuyen tu Analog sang digital
   * @brief(Tin hieu da duoc lay mau)
   */
  float w0 = 2.0f * M_PI * notch_freq / notch_sample_rate; //
  float alpha = sin(w0) / (2.0f * q_param);

  //Chuan hoa he so 
  filter->a0 = 1.0f + alpha;
  filter->a1 = -2.0f * cos(w0);
  filter->a2 = 1.0f - alpha;
  filter->b0 = 1.0f;
  filter->b1 = -2.0f * cos(w0);
  filter->b2 = 1.0f;

  //Chia toan bo he so cho a0
  filter->b0 /= filter->a0;
  filter->b1 /= filter->a0;
  filter->b2 /= filter->a0;
  filter->a1 /= filter->a0;
  filter->a2 /= filter->a0;

  //Khoi tao bo nho trang thai
  filter->x1 = filter->x2 = 0.0f;
  filter->y1 = filter->y2 = 0.0f;
}

//Ham loc Notch 2 tang
float notch_filter_apply(notch_filter_t *filter1, notch_filter_t *filter2, float input_sample){
  float output1, output2;

  //Ap dung bo loc Notch thu nhat (50Hz)
  output1 = (filter1->b0 * input_sample) + (filter1->b1 * filter1->x1) + (filter1->b2 * filter1->x2)
              - (filter1->a1 * filter1->y1) - (filter1->a2 * filter1->y2);
  
  //Kiem tra loi vo cuc hoac loi NaN
  if(isnan(output1) || isinf(output1)){
    output1 = 0.0f; //Neu co loi, dat ve 0.0
  }
  //Cap nhap trang thai bo loc 1
  filter1->x2 = filter1->x1;
  filter1->x1 = input_sample;
  filter1->y2 = filter1->y1;
  filter1->y1 = output1;

  //Ap dung bo loc Notch thu hai (100Hz)
  output2 = (filter2->b0 * output1) + (filter2->b1 * filter2->x1) + (filter2->b2 * filter2->x2)
              - (filter2->a1 * filter2->y1) - (filter2->a2 * filter2->y2);

  //Cap nhat trang thai bo loc 2
  filter2->x2 = filter2->x1;
  filter2->x1 = output1;
  filter2->y2 = filter2->y1;
  filter2->y1 = output2;

  //Kiem tra loi NaN hoac vo cuc 
  if(isnan(output2) || isinf(output2)){
    output2 = 0.0f;
  }
  //Tra lai gia tri da loc
  return output2;
}

void butterworth_filter_init(butterworth_filter_t *filter, float sample_rate, float cutoff_freq){
  float w0 = 2.0f * M_PI * cutoff_freq / sample_rate;
  float sn = sin(w0);
  float cs = cos(w0);
  float alpha = sn / sqrtf(2.0f);

  filter->a_0 = 1.0f + alpha;
  filter->a_1 = -2.0f * cs;
  filter->a_2 = 1.0f - alpha;
  filter->b_0 = 1.0f - cs;
  filter->b_1 = 1.0f - cs;

  //Chia toan bo he so cho a0
  filter->b_0 /= (2.0f * filter->a_0);
  filter->b_1 /= filter->a_0;
  filter->b_2 = filter->b_0;
  filter->a_1 /= filter->a_0;
  filter->a_2 /= filter->a_0;

  //Khoi tao bo nho trang thai
  filter->x_1 = filter->x_2 = 0.0f;
  filter->y_1 = filter->y_2 = 0.0f;

}

float butterworth_filter_apply(butterworth_filter_t *filter, float input_sample){
  float output = (filter->b_0 * input_sample) + (filter->b_1 * filter->x_1) + (filter->b_2 * filter->x_2)
                  - (filter->a_1 * filter->y_1) - (filter->a_2 * filter->y_2);
  
  //Cap nhat trang thai bo loc 
  filter->x_2 = filter->x_1;
  filter->x_1 = input_sample;
  filter->y_2 = filter->y_1;
  filter->y_1 = output;

  return output;
}

int16_t filter_process(int16_t input_sample){
  float temp = (float)input_sample; //Ep tu 16-bit sang float
  temp = notch_filter_apply(&notch_filter1, &notch_filter2, temp); //Loc qua Notch
  temp = butterworth_filter_apply(&butterworth_filter, temp); //Loc qua butterworth

  return (int16_t)temp; 
}