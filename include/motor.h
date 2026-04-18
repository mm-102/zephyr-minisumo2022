#ifndef MOTOR_H
#define MOTOR_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/devicetree.h>

enum motor_operation {
    MOTOR_STOP,
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_BREAK
};

struct motor {
    struct pwm_dt_spec l1;
    struct pwm_dt_spec r1;
    struct gpio_dt_spec l2;
    struct gpio_dt_spec r2;
    float power;
    enum motor_operation operation;
};

#define MOTOR_DT_DEFINE(node_id) { \
    .l1 = PWM_DT_SPEC_GET_BY_NAME(node_id, l1), \
    .r1 = PWM_DT_SPEC_GET_BY_NAME(node_id, r1), \
    .l2 = GPIO_DT_SPEC_GET(node_id, l2_gpios), \
    .r2 = GPIO_DT_SPEC_GET(node_id, r2_gpios), \
    .power = 0.4f, \
    .operation = MOTOR_STOP \
}

int motor_init(struct motor *m);
void motor_set_power(struct motor *m, float power);
void motor_set_operation(struct motor *m, enum motor_operation op);

#endif /* MOTOR_H */