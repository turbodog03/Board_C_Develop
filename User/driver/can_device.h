#include "main.h"
#define FILTER_BUF 5
/**
  * @brief     ��������ṹ��
  */
typedef struct
{
  /* �����ǵ�����ֱ�ӻش������� */
  
  uint16_t ecd;         //����ı�������ֵ
  uint16_t last_ecd;    //��һ�ε���ı�������ֵ
  int16_t  speed_rpm;   //�����ת��ֵ

  /* �����Ǽ�������ĵ��������� */
  int32_t  round_cnt;   //�����ת����Ȧ��
  int32_t  total_ecd;   //�����ת���ܱ�������ֵ
  int32_t  total_angle; //�����ת���ܽǶ�
  
  /* ���µ�������������ʱ���м���������Ժ��� */
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
extern moto_measure_t moto_shoot[2];//0��1�ң�
extern float PowerData[4];
extern int shoot_status;
extern int shoot_cnt;
extern int last_cnt;
/**
  * @brief     CAN1 �жϻص��������ڳ����ʼ��ʱע��
  * @param     recv_id: CAN1 ���յ������� ID
  * @param     data: ���յ��� CAN1 ����ָ��
  */
void can1_recv_callback(uint32_t recv_id, uint8_t data[]);
/**
  * @brief     CAN2 �жϻص��������ڳ����ʼ��ʱע��
  * @param     recv_id: CAN2 ���յ������� ID
  * @param     data: ���յ��� CAN2 ����ָ��
  */
void can2_recv_callback(uint32_t recv_id, uint8_t data[]);

/**
  * @brief     ��������ת��rmp Ȧ��round_cnt 
  *            �ܱ�������ֵtotal_ecd ����ת�ĽǶ�total_angle
  * @param     ptr: ������� moto_measure_t �ṹ��ָ��
  * @param     data: ���յ��ĵ�� CAN ����ָ��
  */
static void encoder_data_handle(moto_measure_t *ptr, uint8_t data[]);
/**
  * @brief     ��õ����ʼƫ��
  * @param     ptr: ������� moto_measure_t �ṹ��ָ��
  * @param     data: ���յ��ĵ�� CAN ����ָ��
  */
static void get_moto_offset(moto_measure_t *ptr, uint8_t data[]);
/**
  * @brief     ���͵��̵���������ݵ����
  */
void send_chassis_moto_current(int16_t current[]);
void send_chassis_moto_zero_current(void);
/**
  * @brief     ������̨����������ݵ����
  */
void send_gimbal_moto_current(int16_t yaw_current, int16_t pit_current);
void send_gimbal_moto_zero_current(void);
/**
  * @brief     ������̨����������ݵ����
  */
void send_shoot_moto_current(int16_t left_current,int16_t right_current, int16_t pit_current);
void sendSuperCap(void);
/**
  * @brief     ���͹�����Ϣ�����ݹ����
 */
void PowerDataResolve(uint8_t data[]);
