#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>
#include <motor.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/* Instantiate BOTH motors using the Device Tree macro */
struct motor left_motor  = MOTOR_DT_DEFINE(DT_NODELABEL(motor1));
struct motor right_motor = MOTOR_DT_DEFINE(DT_NODELABEL(motor2));

int main(void)
{
    LOG_INF("Starting Robot Motor Demo...");

    /* Initialize both motors */
    if (motor_init(&left_motor) != 0 || motor_init(&right_motor) != 0) {
        LOG_ERR("Error: Motor initialization failed!");
        return -1;
    }

    LOG_INF("Motors ready. Moving forward...");

    /* Drive forward at 60% power */
    motor_set_power(&left_motor, 0.6f);
    motor_set_power(&right_motor, 0.6f);
    motor_set_operation(&left_motor, MOTOR_FORWARD);
    motor_set_operation(&right_motor, MOTOR_FORWARD);

    k_sleep(K_SECONDS(2));

    LOG_INF("Turning right (skid steer)...");

    /* Skid Steer Right (Left goes forward, Right goes backward) */
    motor_set_operation(&right_motor, MOTOR_BACKWARD);

    k_sleep(K_SECONDS(1));

    LOG_INF("Full Stop.");
    
    /* Full Stop */
    motor_set_operation(&right_motor, MOTOR_STOP);
    motor_set_operation(&left_motor, MOTOR_BACKWARD);
    k_sleep(K_SECONDS(1));

    while(1){
        if(left_motor.operation == MOTOR_FORWARD)
            motor_set_operation(&left_motor, MOTOR_BACKWARD);
        else
            motor_set_operation(&left_motor, MOTOR_FORWARD);
        k_sleep(K_SECONDS(1));
    }

    return 0;
}