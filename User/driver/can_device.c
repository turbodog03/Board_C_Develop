#include "main.h"
#include "bsp_can.h"
#include "can_device.h"

#define SHOT_SUCCESS_FRIC_WHEEL_SPEED  6800  //发射成功摩擦轮会减速
#define SHOT_ABLE_FRIC_WHEEL_SPEED  6000

/* 云台电机 */
moto_measure_t moto_pit;
moto_measure_t moto_yaw;
/* 拨弹电机 */
moto_measure_t moto_trigger;
/* 底盘电机 */
moto_measure_t moto_chassis[4];
/* 3508摩擦轮电机 */
moto_measure_t moto_shoot[2];//0左，1右；
/* 外围模块测试电机 */
moto_measure_t moto_test;
//电容功率数据
float PowerData[4];
//发射状态，平时为0，每成功发射一发跳变一次
int shoot_status = 0;
int last_state = 0;
int shoot_cnt = 0;
int last_cnt = 0;

//can中断回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        CAN_RxHeaderTypeDef rx_header;
        uint8_t rx_data_can1[8];
			
				HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data_can1);
				switch (rx_header.StdId)
					{
					case CAN_3508_M1_ID:
					{
						moto_chassis[0].msg_cnt++ <= 50 ? get_moto_offset(&moto_chassis[0], rx_data_can1) : \
						encoder_data_handle(&moto_chassis[0], rx_data_can1);

					}
					break;
					case CAN_3508_M2_ID:
					{
						moto_chassis[1].msg_cnt++ <= 50 ? get_moto_offset(&moto_chassis[1], rx_data_can1) : \
						encoder_data_handle(&moto_chassis[1], rx_data_can1);

					}
					break;

					case CAN_3508_M3_ID:
					{
						moto_chassis[2].msg_cnt++ <= 50 ? get_moto_offset(&moto_chassis[2], rx_data_can1) : \
						encoder_data_handle(&moto_chassis[2], rx_data_can1);

					}
					break;
					case CAN_3508_M4_ID:
					{
						moto_chassis[3].msg_cnt++ <= 50 ? get_moto_offset(&moto_chassis[3], rx_data_can1) : \
						encoder_data_handle(&moto_chassis[3], rx_data_can1);

					}
					break;
					case CAN_YAW_MOTOR_ID:
					{
						encoder_data_handle(&moto_yaw, rx_data_can1);

					}
					break;
					case CAN_PIT_MOTOR_ID:
					{
						encoder_data_handle(&moto_pit, rx_data_can1);

					}
					break;
					case CAN_SUPERCAP_RECV:
						PowerDataResolve(rx_data_can1);
					default:
					{
					}
					break;
				}
		}
		else if(hcan->Instance == CAN2)
    {
        CAN_RxHeaderTypeDef rx_header2;
        uint8_t rx_data_can2[8];

        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header2, rx_data_can2);

        switch (rx_header2.StdId)
        {
            case CAN_FRIC_MOTOR_ID_1:
						{
						moto_shoot[0].msg_cnt++ <= 50 ? get_moto_offset(&moto_shoot[0], rx_data_can2) : \
						encoder_data_handle(&moto_shoot[0], rx_data_can2);
						//通过转速变化判断是否有子弹射出,同时计算射出数量
						last_state = shoot_status;
						if(-moto_shoot[0].speed_rpm < SHOT_SUCCESS_FRIC_WHEEL_SPEED && -moto_shoot[0].speed_rpm > SHOT_ABLE_FRIC_WHEEL_SPEED){
							shoot_status = 1;
						}
						else{
							shoot_status = 0;
						}
						if(last_state == 0 && shoot_status == 1){
							shoot_cnt++;
						}
					}
						break;
						case CAN_FRIC_MOTOR_ID_2:
						{
							moto_shoot[1].msg_cnt++ <= 50 ? get_moto_offset(&moto_shoot[1], rx_data_can2) : \
							encoder_data_handle(&moto_shoot[1], rx_data_can2);

						}
						break;
						//拨弹电机
						case CAN_TRIGGER_MOTOR_ID:
						{
							moto_trigger.msg_cnt++;
							moto_trigger.msg_cnt <= 10 ? get_moto_offset(&moto_trigger, rx_data_can2) : encoder_data_handle(&moto_trigger, rx_data_can2);

						}
						break;
						default:	
						break;
					}
				}
			}
			
        
				
/**
  * @brief     获得电机初始偏差
  * @param     ptr: 电机参数 moto_measure_t 结构体指针
  * @param     data: 接收到的电机 CAN 数据指针
  */
static void get_moto_offset(moto_measure_t *ptr, uint8_t data[])
{
  ptr->ecd        = (uint16_t)(data[0] << 8 | data[1]);
  ptr->offset_ecd = ptr->ecd;
}

/**
  * @brief     计算电机的转速rmp 圈数round_cnt 
  *            总编码器数值total_ecd 总旋转的角度total_angle
  * @param     ptr: 电机参数 moto_measure_t 结构体指针
  * @param     data: 接收到的电机 CAN 数据指针
  */
static void encoder_data_handle(moto_measure_t *ptr, uint8_t data[])
{
  int32_t temp_sum = 0;
  
  ptr->last_ecd      = ptr->ecd;
	//转子机械角度
  ptr->ecd           = (uint16_t)(data[0] << 8 | data[1]);
  //转子转速
  ptr->speed_rpm     = (int16_t)(data[2] << 8 | data[3]);
	//转矩电流没有处理
  if (ptr->ecd - ptr->last_ecd > 4096)
  {
    ptr->round_cnt--;
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd - 8192;
  }
  else if (ptr->ecd - ptr->last_ecd < -4096)
  {
    ptr->round_cnt++;
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd + 8192;
  }
  else
  {
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd;
  }

  ptr->total_ecd = ptr->round_cnt * 8192 + ptr->ecd - ptr->offset_ecd;
  ptr->total_angle = ptr->total_ecd * 360 / 8192;
  
  
  ptr->rate_buf[ptr->buf_cut++] = ptr->ecd_raw_rate;
  if (ptr->buf_cut >= FILTER_BUF)
    ptr->buf_cut = 0;
  for (uint8_t i = 0; i < FILTER_BUF; i++)
  {
    temp_sum += ptr->rate_buf[i];
  }
  ptr->filter_rate = (int32_t)(temp_sum/FILTER_BUF);
}


/**
* @brief     给底盘电机发送零电流
  */
void send_chassis_moto_zero_current(void)
{
  static uint8_t data[8];
  
  data[0] = 0;
  data[1] = 0;
  data[2] = 0;
  data[3] = 0;
  data[4] = 0;
  data[5] = 0;
  data[6] = 0;
  data[7] = 0;
  
  write_can(CHASSIS_CAN, CAN_CHASSIS_ID, data);
}

/**
  * @brief     发送底盘电机电流数据到电调
  */
void send_chassis_moto_current(int16_t current[])
{
  static uint8_t data[8];
  
  data[0] = current[0] >> 8;
  data[1] = current[0];
  data[2] = current[1] >> 8;
  data[3] = current[1];
  data[4] = current[2] >> 8;
  data[5] = current[2];
  data[6] = current[3] >> 8;
  data[7] = current[3];
	
  write_can(CHASSIS_CAN, CAN_CHASSIS_ID, data);
}

/**
  * @brief     发送云台电机电流数据到电调
  */
extern int16_t trigger_moto_current;
void send_gimbal_moto_current(int16_t yaw_current, int16_t pit_current)
{
  static uint8_t data[8];
	
  data[0] = yaw_current >> 8;
  data[1] = yaw_current;
  data[2] = pit_current >> 8;
  data[3] = pit_current;
  //data[4] = trigger_current >> 8;
  //data[5] = trigger_current;
  data[6] = 0;
  data[7] = 0;
  
  write_can(GIMBAL_CAN, CAN_GIMBAL_ID, data);
}
void send_gimbal_moto_zero_current(void)
{
  static uint8_t data[8];
  
  data[0] = 0;
  data[1] = 0;
  data[2] = 0;
  data[3] = 0;
  data[4] = 0;
  data[5] = 0;
  data[6] = 0;
  data[7] = 0;
  
  write_can(GIMBAL_CAN, CAN_GIMBAL_ID, data);
}

void send_shoot_moto_current(int16_t left_current,int16_t right_current, int16_t trigger_current)
{
  static uint8_t data[8];
	
  data[0] = left_current >> 8;
  data[1] = left_current;
  data[2] = right_current >> 8;
  data[3] = right_current;
  data[4] = trigger_current >> 8;
  data[5] = trigger_current;
  data[6] = 0;
  data[7] = 0;
  
  write_can(USER_CAN2, CAN_CHASSIS_ID, data);
}
//超级电容
void sendSuperCap(void)
{
	uint16_t temPower =9000;//功率设定步进0.01W，范围为3000-13000（30W-130W）
	uint8_t sendbuf[8];//发送的数据内容
	sendbuf[0]=temPower >> 8;
	sendbuf[1]=temPower;
	write_can(USER_CAN1, CAN_SUPER_CAP_ID, sendbuf);
}
void PowerDataResolve(uint8_t data[])
{
	extern float PowerData[4];
	uint16_t *pPowerData =(uint16_t *) data;
	PowerData[0]=(float)pPowerData[0]/100.f;//输入电压
	PowerData[1]=(float)pPowerData[1]/100.f;//电容电压
	PowerData[2]=(float)pPowerData[2]/100.f;//输入电流
	PowerData[3]=(float)pPowerData[3]/100.f;//设定功率
}
