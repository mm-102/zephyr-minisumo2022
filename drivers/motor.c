#include "motor.h"

static void apply_hardware_state(struct motor *m)
{
    uint32_t period = m->l1.period; 
    uint32_t pulse_off = period; // 100% duty cycle (inverted)
    uint32_t pulse_active = period - (uint32_t)(period * m->power);

    switch (m->operation) {
        case MOTOR_STOP:
            pwm_set_pulse_dt(&m->l1, pulse_off);
            pwm_set_pulse_dt(&m->r1, pulse_off);
            gpio_pin_set_dt(&m->l2, 1);
            gpio_pin_set_dt(&m->r2, 1);
            break;

        case MOTOR_FORWARD:
            pwm_set_pulse_dt(&m->l1, pulse_active);
            pwm_set_pulse_dt(&m->r1, pulse_off);
            gpio_pin_set_dt(&m->l2, 1);
            gpio_pin_set_dt(&m->r2, 0);
            break;

        case MOTOR_BACKWARD:
            pwm_set_pulse_dt(&m->l1, pulse_off);
            pwm_set_pulse_dt(&m->r1, pulse_active);
            gpio_pin_set_dt(&m->l2, 0);
            gpio_pin_set_dt(&m->r2, 1);
            break;

        case MOTOR_BREAK:
            pwm_set_pulse_dt(&m->l1, pulse_off);
            pwm_set_pulse_dt(&m->r1, pulse_off);
            gpio_pin_set_dt(&m->l2, 0);
            gpio_pin_set_dt(&m->r2, 0);
            break;
    }
}

int motor_init(struct motor *m)
{
    if (!pwm_is_ready_dt(&m->l1) || !pwm_is_ready_dt(&m->r1) ||
        !gpio_is_ready_dt(&m->l2) || !gpio_is_ready_dt(&m->r2)) {
        return -ENODEV;
    }

    gpio_pin_configure_dt(&m->l2, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&m->r2, GPIO_OUTPUT_ACTIVE);
    apply_hardware_state(m);
    return 0;
}

void motor_set_operation(struct motor *m, enum motor_operation op)
{
    if (m->operation != op) {
        k_sleep(K_MSEC(5));
    }
    
    m->operation = op;
    apply_hardware_state(m);
}

void motor_set_power(struct motor *m, float power)
{
    if (power < 0.0f) power = 0.0f;
    if (power > 1.0f) power = 1.0f;
    
    m->power = power;
    if (m->operation == MOTOR_FORWARD || m->operation == MOTOR_BACKWARD) {
        apply_hardware_state(m);
    }
}