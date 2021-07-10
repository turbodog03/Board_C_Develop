
#ifndef BSP_BSP_CAN_H
#define BSP_BSP_CAN_H
#include "main.h"
extern void can_filter_init(void);


//#define CHASSIS_CAN hcan1
//#define GIMBAL_CAN hcan2
#define CAN_DATA_SIZE  8

//电机rmp 变化成 旋转速度的比例
#define MOTOR_RPM_TO_SPEED          0.00290888208665721596153948461415f
//电机ecd 变化成 角度的比例
#define MOTOR_ECD_TO_ANGLE          0.000021305288720633905968306772076277f

/* CAN send and receive ID */
/**
  * @brief     CAN 设备发送和接收 ID 枚举
  */


typedef enum
{
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,

    CAN_YAW_MOTOR_ID = 0x205,
    CAN_PIT_MOTOR_ID = 0x206,
} can1_msg_id_e;

typedef enum
{

    CAN_FRIC_MOTOR_ID_1 = 0x201,
    CAN_FRIC_MOTOR_ID_2 = 0x202,
    CAN_TRIGGER_MOTOR_ID = 0x203,
		CAN_SUPERCAP_RECV    = 0x211,
} can2_msg_id_e;

typedef enum
{

  //发送ID
  CAN_CHASSIS_ID       = 0x200,
  CAN_SUPER_CAP_ID      = 0X210,
  CAN_GIMBAL_ID        = 0x1ff,

} can_tx_id_e;


//rm motor data
typedef struct
{
    uint16_t ecd;//转子机械角度，范围为0-8191，对应角度0-360
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperature;
    int16_t last_ecd;
} motor_measure_t;

extern motor_measure_t motor_chassis[9];

/**
  * @brief          send control current of motor (0x205, 0x206, 0x207, 0x208)
  * @param[in]      yaw: (0x205) 6020 motor control current, range [-30000,30000]
  * @param[in]      pitch: (0x206) 6020 motor control current, range [-30000,30000]
  * @param[in]      shoot: (0x207) 2006 motor control current, range [-10000,10000]
  * @param[in]      rev: (0x208) reserve motor control current
  * @retval         none
  */
/**
  * @brief          can1发送电机控制电流(0x205,0x206,0x207,0x208)
  * @param[in]      yaw: (0x205) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      pitch: (0x206) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      shoot: (0x207) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      rev: (0x208) 保留，电机控制电流
  * @retval         none
  */
extern void CAN_cmd_gimbal(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev);

/**
  * @brief          can2发送电机控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      trigger: (0x201) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      fric: (0x202、0x203) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      rev: (0x204) 保留，电机控制电流
  * @retval         none
  */
extern void CAN_cmd_shoot(int16_t fric, int16_t trigger);

/**
  * @brief          send CAN packet of ID 0x700, it will set chassis motor 3508 to quick ID setting
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          发送ID为0x700的CAN包,它会设置3508电机进入快速设置ID
  * @param[in]      none
  * @retval         none
  */


extern void CAN_cmd_chassis_reset_ID(void);

/**
  * @brief          send control current of motor (0x201, 0x202, 0x203, 0x204)
  * @param[in]      motor1: (0x201) 3508 motor control current, range [-16384,16384]
  * @param[in]      motor2: (0x202) 3508 motor control current, range [-16384,16384]
  * @param[in]      motor3: (0x203) 3508 motor control current, range [-16384,16384]
  * @param[in]      motor4: (0x204) 3508 motor control current, range [-16384,16384]
  * @retval         none
  */
/**
  * @brief          can1发送电机控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor2: (0x202) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor3: (0x203) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor4: (0x204) 3508电机控制电流, 范围 [-16384,16384]
  * @retval         none
  */
extern void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

/**
  * @brief          return the yaw 6020 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回yaw 3508电机数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
extern const motor_measure_t *get_yaw_gimbal_motor_measure_point(void);

/**
  * @brief          return the pitch 6020 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回pitch 3508电机数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
extern const motor_measure_t *get_pitch_gimbal_motor_measure_point(void);

/**
  * @brief          return the trigger 2006 motor data point
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回拨弹电机 3508电机数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
extern const motor_measure_t *get_trigger_motor_measure_point(void);

const motor_measure_t *get_fric1_motor_measure_point(void);
const motor_measure_t *get_fric2_motor_measure_point(void);
/**
  * @brief          return the chassis 3508 motor data point
  * @param[in]      i: motor number,range [0,3]
  * @retval         motor data point
  */
/**
  * @brief          返回底盘电机 3508电机数据指针
  * @param[in]      i: 电机编号,范围[0,3]
  * @retval         电机数据指针
  */
extern const motor_measure_t *get_chassis_motor_measure_point(uint8_t i);

///////////////////////////////////////////////////////////

// 老代码写bsp（施工中）
//CAN 设备端口定义
#define USER_CAN1         1  //CAN1
#define USER_CAN2         2  //CAN2
#define CHASSIS_CAN       1  //底盘电机使用CAN1
#define GIMBAL_CAN        1  //云台电机使用CAN1
#define TRIGGER_CAN       1  //拨弹电机使用CAN1

//CAN 设备相关函数
/**
  * @brief     CAN 设备初始化
  */
void can_device_init(void);
/**
  * @brief     发送 CAN 数据
  * @param     can_id: CAN 设备 ID，只有 CAN1 或者 CAN2
  * @param     send_id: 发送数据 ID
  * @param     send_data: 发送数据指针，大小为 8 位
  */
void write_can(uint8_t can_id, uint32_t send_id, uint8_t send_data[]);


#endif //BSP_BSP_CAN_H

