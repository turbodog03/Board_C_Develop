#include "main.h"
#define FILTER_BUF 5
/**
  * @brief     电机参数结构体
  */
typedef struct
{
  /* 以下是电机电调直接回传的数据 */
  
  uint16_t ecd;         //电机的编码器数值
  uint16_t last_ecd;    //上一次电机的编码器数值
  int16_t  speed_rpm;   //电机的转速值

  /* 以下是计算出来的电机相关数据 */
  int32_t  round_cnt;   //电机旋转的总圈数
  int32_t  total_ecd;   //电机旋转的总编码器数值
  int32_t  total_angle; //电机旋转的总角度
  
  /* 以下电机计算相关数据时的中间变量，可以忽略 */
  uint16_t offset_ecd;
  uint32_t msg_cnt;
  int32_t  ecd_raw_rate;
  int32_t  rate_buf[FILTER_BUF];
  uint8_t  buf_cut;
  int32_t  filter_rate;
} moto_measure_t;


extern moto_measure_t moto_chassis[];
extern moto_measure_t moto_yaw;
extern moto_measure_t moto_pit;
extern moto_measure_t moto_trigger;
extern moto_measure_t moto_test;
extern moto_measure_t moto_shoot[2];//0左，1右；
extern float PowerData[4];
extern int shoot_status;
extern int shoot_cnt;
extern int last_cnt;
/**
  * @brief     CAN1 中断回调函数，在程序初始化时注册
  * @param     recv_id: CAN1 接收到的数据 ID
  * @param     data: 接收到的 CAN1 数据指针
  */
void can1_recv_callback(uint32_t recv_id, uint8_t data[]);
/**
  * @brief     CAN2 中断回调函数，在程序初始化时注册
  * @param     recv_id: CAN2 接收到的数据 ID
  * @param     data: 接收到的 CAN2 数据指针
  */
void can2_recv_callback(uint32_t recv_id, uint8_t data[]);

/**
  * @brief     计算电机的转速rmp 圈数round_cnt 
  *            总编码器数值total_ecd 总旋转的角度total_angle
  * @param     ptr: 电机参数 moto_measure_t 结构体指针
  * @param     data: 接收到的电机 CAN 数据指针
  */
static void encoder_data_handle(moto_measure_t *ptr, uint8_t data[]);
/**
  * @brief     获得电机初始偏差
  * @param     ptr: 电机参数 moto_measure_t 结构体指针
  * @param     data: 接收到的电机 CAN 数据指针
  */
static void get_moto_offset(moto_measure_t *ptr, uint8_t data[]);
/**
  * @brief     发送底盘电机电流数据到电调
  */
void send_chassis_moto_current(int16_t current[]);
void send_chassis_moto_zero_current(void);
/**
  * @brief     发送云台电机电流数据到电调
  */
void send_gimbal_moto_current(int16_t yaw_current, int16_t pit_current);
void send_gimbal_moto_zero_current(void);
/**
  * @brief     发送云台电机电流数据到电调
  */
void send_shoot_moto_current(int16_t left_current,int16_t right_current, int16_t pit_current);
void sendSuperCap(void);
/**
  * @brief     发送功率信息到电容管理板
 */
void PowerDataResolve(uint8_t data[]);
